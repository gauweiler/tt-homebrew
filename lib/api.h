#ifndef __API_H__
#define __API_H__

typedef struct {                                                                                               // this seems to be the same on 2N and 3L, but fpAkOidPara differs
    void *(*malloc)();                                                                                            // 0
    void (*free)();                                                                                            // 1 both (0 & 1) end up in the same code
    void (*printf)(unsigned char *);                                                                           // 2
    int (*is_audio_playing)();                                                                                 // 3
    void (*tbd4)();                                                                                            // 4 sets some function pointers
    int (*open)(unsigned short *, unsigned int, unsigned int);                                                 // 5
    int (*read)(int filehandle, void *buff, unsigned int size);                                                // 6 
    int (*write)(int filehandle, void *buff, unsigned int size);                                               // 7
    int (*close)(int filehandle);                                                                              // 8
    int (*seek)(int, unsigned int, int allways_is_zero);                                                       // 9
    void (*tbd10)();                                                                                           // 10
    int (*play_sound)(int filehandle, unsigned int offset, unsigned int size);  // 11
    char *(*returns_if_p_audPlayer_is_null)();                                                                 // 12
    unsigned char *(fpAkOidPara);                                                                              // 13
    int(*p_filehandle_current_gme);                                                                            // 14
    unsigned short *(gme_registers);
    int(*tbd16);
    void (*IMP_acces_from_gme_gametable_offset_maybe_init)();
    int(*tbd18);
    void (*tbd19)();
    int(*tbd20);    // = 0
    int(*tbd21);    // = 0x3004
    int(*tbd22);    // = 0xac19
    int(*tbd23);    // = 0 (1byte)
    int(*tbd24);    // = 1 (next byte)
    int(*tbd25);    // = 1 (next byte)
    int(*tbd26);    // = 1 (next byte)
    void (*imp_maybe_xor)(); // no code
    void (*parse_new_gme_header)();
    void (*inits_resets_the_gme_registers_to_default)();
    void (*parses_start_and_end_oid)();
    void (*parse_play_script_into_lines)(unsigned short oid);  // ig unsigned short
    void (*finds_script_line_that_meets_condition)();
    void (*check_oid_script_condition)();
    void (*parses_action_part_of_script_line)();
    void (*executes_play_script_comman)();
    void (*tbd36)();
    void (*parses_media_offsets_and_size)();
    void (*also_parses_media_offsets_and_size)();
    void (*sets_script_related_stuff_to_zero)();
    void (*parses_og_game_table)();
    void (*parses_aditional_script_table)();
    void (*checks_product_id_and_ravensburg_id)();
    void (*something_mod)();
    int(*tbd44); // filehandle
    int(*tbd45);
    int(*tbd46);
    int(*tbd47); // condition number
    int(*tbd48);
    int(*gme_register_count);
    int(*tbd50);
    int(*tbd51);
    int(*tbd52);
    int(*tbd53);
    int(*tbd54);
    int(*tbd55);
    int(*tbd56);
    int(*tbd57);
    int(*tbd58);
    int(*tbd59);
    int(*tbd60);
    int(*tbd61);
    int(*tbd62);
    int(*tbd63);
    int(*tbd64);
    int(*tbd65);
    int(*tbd66);
    int(*tbd67);
    int(*tbd68); // product id
    int(*tbd69);
    int(*tbd70);
    int(*tbd71);
    int(*tbd72);
    unsigned short *(also_gme_registers);
    int(*tbd74);    // = 0x238b
    int(*tbd75);
    int(*tbd76);
    int(*tbd77);
    int(*tbd78);
    int(*tbd79); // vllt last played sound id
    int(*tbd80);
    int(*tbd81);
    int(*tbd82);
    int(*tbd83);
    int(*tbd84);
    int(*tbd85);
    int(*tbd86);
    int(*tbd87);
    int(*tbd88);
    int(*tbd89);
    int(*tbd90);
    int(*tbd91);
    int(*tbd92);
    int(*tbd93);
    int(*tbd94);
    int(*tbd95);
    int(*tbd96);
    int(*tbd97);
    int(*tbd98);
    void (*play_chomp_voice)();
    void (*get_random_number_or_counter)();
    void (*maybe_mkFile_maybe_delete)();
    void (*get_random)(); // somthing in bios
    void (*tbd103)(); // functions from here on (including this) is only avalibel on create pens 
    void (*returns_booc_rec_str)();
    void (*tbd105)();
    void (*broken_record_sound)();
    void (*also_record_sound)();
    void (*record_sound)(int rec_id, int tbd_is_10);
    void (*more_record_sound)();
    void (*end_of_record_sound)();
    void (*used_in_rec_main)();
    void (*play_recording)(int rec_id);
    void (*tbd113)();
    void (*tbd114)();
    void (*select_recording_for_playback)();
    void (*tbd116)();
    void (*spam_print_and_sounds)();
    void (*tbd118)();
    void (*tbd119)();
    void (*tbd120)();
    void (*something_with_tomplay_wav)();
    void (*idk_but_can_say_record_full)();
    void (*used_in_rec_main2)();
    void (*IMP_has_record_filepath_string)();
    void (*tbd125)();
    void (*tbd126)();
    void (*something_play_atmo_short)();
    void (*tbd128)();
    void (*if_stuff_not_null_executes_funcs_and_sets_it_null)();
    void (*tbd130)();
    void (*tbd131)();
    void (*create_recoring_folder)(int product_id);
    void (*tbd133)();
    void (*tbd134)();
    void (*tbd135)();
    void (*tbd136)();
    void (*other_book_recc_str)();
    void (*tbd138)();
    void (*tbd139)();
    void (*tbd140)();
} system_api;

// typedef struct {
//     int ig_flag;
//     int func_pointer;
//     int pos;
// } file;

// #define build_for_2N
#ifdef build_for_2N
// #pragma message("Compiling for 2N")
#define First_time_exec 0xdec
#define anotherMain_last_maybe_exit 0xdee
#define new_oid 0xded
#define oid_offset 0x4
#define otherMain_equals_100 0x58

#define next_game 0x131
#define start_next_game 0x12f

#define BINARY_OFFSET 0x08132000
#define SAVEDATA 0x08141000
#define START_PROG_BIN 0x08009000
#else
// #pragma message("Compiling for 3L")
#define First_time_exec 0x1318
#define anotherMain_last_maybe_exit 0x131a
#define new_oid 0x1319
#define oid_offset 0xc
#define otherMain_equals_100 0x6

#define next_game 0x69d
#define start_next_game 0x69b

#define BINARY_OFFSET 0x00930000
#define SAVEDATA 0x00940000
#define START_PROG_BIN 0x00829000
#endif

#endif