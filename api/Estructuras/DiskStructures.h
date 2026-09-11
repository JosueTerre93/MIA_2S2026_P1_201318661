#ifndef DISK_STRUCTURES_H
#define DISK_STRUCTURES_H

#include <ctime>

struct Partition
{
    char part_status;
    char part_type;
    char part_fit;

    int part_start;
    int part_s;

    char part_name[16];
    int part_correlative;
    char part_id[4];
};

struct MBR
{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_dsk_signature;
    char dsk_fit;

    Partition mbr_partitions[4];
};

struct EBR
{
    char part_mount;
    char part_fit;

    int part_start;
    int part_s;
    int part_next;

    char part_name[16];
};

#endif