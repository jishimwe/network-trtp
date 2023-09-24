#include "packet_interface.h"
#include "stdint.h"
#include <stdlib.h>
#include <memory.h>
#include <zlib.h>
#include <arpa/inet.h>

//struct __attribute__((__packed__)) pkt {
//    uint8_t window : 5;
//    uint8_t tr : 1;
//    uint8_t type : 2;
//    uint16_t length;
//    uint8_t seqnum;
//    uint32_t timestamp;
//    uint32_t crc1;
//    char * payload;
//    uint32_t crc2;
//};



ssize_t predict_header_length(const pkt_t *pkt) {
    return pkt->payload == NULL ? PKT_MIN_HEADERLEN : PKT_MAX_HEADERLEN;
}

/*
 * Décode des données reçues et crée une nouvelle structure pkt.
 * Le paquet reçu est en network byte-order.
 * La fonction vérifie que:
 * - Le CRC32 du header recu est le même que celui decode a la fin
 *   du header (en considerant le champ tr a 0)
 * - S'il est present, le CRC32 du payload recu est le meme que celui
 *   decode a la fin du payload
 * - Le type du paquet est valide
 * - La longueur du paquet et le champ tr sont valides et coherents
 *   avec le nombre d'octets recus.
 *
 * @data: L'ensemble d'octets constituant le paquet reçu
 * @len: Le nombre de bytes reçus
 * @pkt: Une struct pkt valide
 * @post: pkt est la représentation du paquet reçu
 *
 * @return: Un code indiquant si l'opération a réussi ou représentant
 *         l'erreur rencontrée.
 */
pkt_status_code pkt_decode(const char *data, const size_t len, pkt_t *pkt) {
    int i = 0;
    char cpy_data[len];
    memcpy(cpy_data, data, len);
    //uint32_t * int32_ptr = (uint32_t *) cpy_data;
    if (len == PKT_MIN_HEADERLEN) {
        uint8_t *uint8_ptr = (uint8_t *) pkt;
        uint8_t *uint8_data = (uint8_t *) cpy_data;
        uint8_ptr[0] = uint8_data[0];
        i = 1;
        for (; i < PKT_MIN_HEADERLEN + 2; i++) {
            if (i < 3) {
                uint8_ptr[i] = 0;
            } else {
                uint8_ptr[i] = uint8_data[i - 2];
            }
        }
    } else {
        uint32_t *uint_ptr = (uint32_t *) pkt;
        uint32_t *uint_data = (uint32_t *) cpy_data;
        for (int j = 0; j < 12 / (int) sizeof(uint32_t); j++, i++) {
            uint_ptr[j] = uint_data[i];
        }
    }
    char *buf = (char *) pkt;
    uint16_t *temp_length = (uint16_t *) &buf[1];
    uint32_t *temp_crc1 = (uint32_t *) buf;
    temp_crc1[2] = (uint32_t) ntohl(temp_crc1[2]);
    temp_length[0] = (uint16_t) ntohs(*temp_length);
    if (pkt->type < 2 && pkt->length <= 512) {
        int z = i * 4;
        int l;
        if (pkt->type == PTYPE_FEC) {
            pkt->payload = malloc(MAX_PAYLOAD_SIZE);
            l = MAX_PAYLOAD_SIZE;
        } else {
            pkt->payload = malloc(pkt->length);
            l = pkt->length;
        }
        for (int j = 0; j < l; j++, z++) {
            pkt->payload[j] = cpy_data[z];
        }
        char *crc2_ptr = (char *) &pkt->crc2;
        for (int j = 0; j < 4; ++j, z++) {
            crc2_ptr[j] = cpy_data[z];
        }
        pkt->crc2 = ntohl(pkt->crc2);
    }


    uint8_t TR_cpy = pkt->tr;
    pkt->tr = 0;
    uint32_t result_crc = (uint32_t) crc32(0, (const unsigned char *) data, 8);
    pkt->tr = TR_cpy;
    if (result_crc != pkt->crc1 && pkt->tr == 0) {
        return E_CRC;
    }
//    if(pkt->type == PTYPE_DATA && (pkt->crc2 == 0 || pkt->length == 0)){
//        return E_TYPE;
//    }
//    if(pkt->type == PTYPE_ACK && (pkt->crc2 != 0 || pkt->length != 0)){
//        return E_TYPE;
//    }
//    if(pkt->type == PTYPE_NACK && (pkt->tr == 0 || pkt->length != 0)){
//        return E_TYPE;
//    }
    if (pkt->length > 512) {
        return E_LENGTH;
    }
    if (pkt->type == PTYPE_FEC && pkt->length != 512) {
        return E_LENGTH;
    }
    if (pkt->type == PTYPE_DATA) {
        result_crc = (uint32_t) crc32(0, (const unsigned char *) pkt->payload, pkt->length);
        if (result_crc != pkt->crc2) {
            return E_NOMEM;
        }
    }
    return PKT_OK;
}


/*
 * Encode une struct pkt dans un buffer, prêt à être envoyé sur le réseau
 * (c-à-d en network byte-order), incluant le CRC32 du header et
 * eventuellement le CRC32 du payload si celui-ci est non nul.
 * La fonction pkt doit calculer les champs CRC elle-même, car
 * ils ne sont pas nécessairements présents dans pkt.
 *
 * @pkt: La structure à encoder
 * @buf: Le buffer dans lequel la structure sera encodée
 * @len: La taille disponible dans le buffer
 * @len-POST: Le nombre de d'octets écrit dans le buffer
 * @return: Un code indiquant si l'opération a réussi ou E_NOMEM si
 *         le buffer est trop petit.
 */
pkt_status_code pkt_encode(const pkt_t *pkt, char *buf, size_t *len) {
    if (*len < sizeof(pkt_t)) {
        return E_NOMEM;
    }
    pkt_t cpy;
    memcpy(&cpy, pkt, sizeof(pkt_t));
    memcpy(cpy.payload, pkt->payload, pkt->length);
    cpy.tr = 0;
    if (cpy.payload != NULL) {
        cpy.crc2 = (uint32_t) crc32(0, (const unsigned char *) pkt->payload, pkt->length);
    }
    cpy.tr = pkt->tr;
    int i = 0;
    char *to_char_ptr = (char *) &cpy;
    char *to_char_crc2 = (char *) &cpy.crc2;
    if (pkt->type == PTYPE_ACK || pkt->type == PTYPE_NACK) // Encoding a (n)ack packet
    {
        buf[0] = to_char_ptr[0];
        for (int j = 1; j < PKT_MIN_HEADERLEN; j++) {
            buf[j] = to_char_ptr[j + 2];
        }
        *len = PKT_MIN_HEADERLEN;
        uint32_t *temp_crc1 = (uint32_t *) &buf[1];
        temp_crc1[1] = (uint32_t) htonl(crc32(0L, (const unsigned char *) pkt, 8));
//        pkt_t *d = pkt_new();
//        pkt_decode(buf, PKT_MIN_HEADERLEN, d);
//        printf("===== ACK SEQ: %d VS PKT SEQNUM : %d ==== BUFFER : %s\n", d->seqnum, pkt->seqnum, buf);
//        printf("===== WINDOW : %d VS %d TYPE : %d VS %d LENGTH : %d VS %d\n", pkt->window, pkt->window, pkt->type, d->type, pkt->length, d->length);
        return PKT_OK;
    }
    for (; i < PKT_MAX_HEADERLEN; ++i) {
        buf[i] = to_char_ptr[i];
    }
    for (int j = 0; j < pkt->length; ++j, i++) {
        buf[i] = pkt->payload[j];
    }
    for (int j = 0; j < (int) sizeof(uint32_t); ++j, i++) {
        buf[i] = to_char_crc2[j];
    }

    uint16_t *temp_length = (uint16_t *) &buf[1];
    temp_length[0] = htons(temp_length[0]);
    uint32_t *temp_crc1 = (uint32_t *) buf;
    temp_crc1[2] = (uint32_t) htonl(crc32(0L, (const unsigned char *) buf, 8));
    uint32_t *temp_crc2 = (u_int32_t *) (buf + 12 + pkt->length);
    *temp_crc2 = htonl(*temp_crc2);
    if (pkt->type == PTYPE_DATA || pkt->type == PTYPE_FEC) {
        *len = 16 + pkt->length;
    } else {
        *len = 12;
    }
    return PKT_OK;
}


//CHECK
pkt_t *pkt_new() {
    pkt_t *ptr = (pkt_t *) malloc(sizeof(pkt_t));
    ptr->payload = NULL;
    if (ptr == NULL) {
        fprintf(stderr, "Error allocating memory in pkt_new()");
        return NULL;
    }
    memset(ptr, 0, sizeof(pkt_t));
    return ptr;
}

//CHECK
void pkt_del(pkt_t *pkt) {
    if(pkt->payload != NULL){
        free(pkt->payload);
        pkt->payload = NULL;
    }
    free(pkt);
}

//CHECK
ptypes_t pkt_get_type(const pkt_t *pkt) {
    if (pkt == NULL) {
        return 0;
    }
    return pkt->type;
}

//CHECK
uint8_t pkt_get_tr(const pkt_t *pkt) {
    if (pkt == NULL) {
        return 0;
    }
    return pkt->tr;
}

//CHECK
uint8_t pkt_get_window(const pkt_t *pkt) {
    if (pkt == NULL) {
        return 0;
    }
    return pkt->window;
}

//CHECK
uint8_t pkt_get_seqnum(const pkt_t *pkt) {
    if (pkt == NULL) {
        return 0;
    }
    return pkt->seqnum;
}

//CHECK
uint16_t pkt_get_length(const pkt_t *pkt) {
    if (pkt == NULL) {
        return 0;
    }
    return pkt->length;
}

//CHECK
uint32_t pkt_get_timestamp(const pkt_t *pkt) {
    if (pkt == NULL) {
        return 0;
    }
    return pkt->timestamp;
}

//CHECK
uint32_t pkt_get_crc1(const pkt_t *pkt) {
    if (pkt == NULL) {
        return 0;
    }
    return pkt->crc1;
}

//CHECK
uint32_t pkt_get_crc2(const pkt_t *pkt) {
    if (pkt == NULL) {
        return 0;
    }
    return pkt->crc2;
}

//CHECK
const char *pkt_get_payload(const pkt_t *pkt) {
    if (pkt == NULL) {
        return NULL;
    }
    return pkt->payload;
}

//CHECK
pkt_status_code pkt_set_type(pkt_t *pkt, const ptypes_t type) {
    if (type >= 0 && type < 4) {
        pkt->type = type;
        return PKT_OK;
    }
    return E_TYPE;
}

//CHECK
pkt_status_code pkt_set_tr(pkt_t *pkt, const uint8_t tr) {
    if (tr == 1 || tr == 0) {
        pkt->tr = tr;
        return PKT_OK;
    }
    return E_TR;
}

//CHECK
pkt_status_code pkt_set_window(pkt_t *pkt, const uint8_t window) {
    if (window > 31) {
        return E_WINDOW;
    }
    pkt->window = window;
    return PKT_OK;
}

//CHECK
pkt_status_code pkt_set_seqnum(pkt_t *pkt, const uint8_t seqnum) {
    if (pkt == NULL) {
        return E_UNCONSISTENT;
    }
    pkt->seqnum = seqnum;
    return PKT_OK;
}

//CHECK
pkt_status_code pkt_set_length(pkt_t *pkt, const uint16_t length) {
    if (length > 512 || length == 0 || pkt == NULL) {
        return E_UNCONSISTENT;
    }
    pkt->length = length;
    return PKT_OK;
}

//CHECK
pkt_status_code pkt_set_timestamp(pkt_t *pkt, const uint32_t timestamp) {
    if (pkt == NULL) {
        return E_UNCONSISTENT;
    }
    pkt->timestamp = timestamp;
    return PKT_OK;
}

//CHECK
pkt_status_code pkt_set_crc1(pkt_t *pkt, const uint32_t crc1) {

    if (pkt == NULL) {
        return E_UNCONSISTENT;
    }
    pkt->crc1 = crc1;
    return PKT_OK;
}

//CHECK
pkt_status_code pkt_set_crc2(pkt_t *pkt, const uint32_t crc2) {
    if (pkt == NULL) {
        return E_UNCONSISTENT;
    }
    pkt->crc2 = crc2;
    return PKT_OK;
}

//CHECK
/* Défini la valeur du champs payload du paquet.
 * @data: Une succession d'octets représentants le payload
 * @length: Le nombre d'octets composant le payload
 * @POST: pkt_get_length(pkt) == length */
pkt_status_code pkt_set_payload(pkt_t *pkt,
                                const char *data,
                                const uint16_t length) {
    if (length == 0 || data == NULL) {
        return E_UNCONSISTENT;
    }
    if (length > 512) {
        return E_LENGTH;
    }
    if (pkt->payload == NULL) {
        pkt->payload = malloc(length);
        if (pkt->payload == NULL) {
            return E_NOMEM;
        }
        memcpy(pkt->payload, data, length);
        pkt->length = length;
    } else {
        free(pkt->payload);
        pkt->length = length;
        pkt->payload = malloc(sizeof(length));
        if (pkt->payload == NULL) {
            return E_NOMEM;
        }
        memcpy(pkt->payload, data, length);
        //CHECK MEMCPY
    }
    return PKT_OK;
}