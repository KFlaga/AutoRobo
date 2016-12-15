#include "MenuTexts.h"

// Saved in flash
const unsigned char  _mt_prime_topmenu[16] __attribute__ ((section(".rodata"))) = {
		'M','e','n','u',' ','N','a','d',
		'r','z','e','d','n','e',' ',' '
};
const unsigned char  _mt_prime_singleinst[16] __attribute__ ((section(".rodata"))) = {
		'P','o','j','e','d','.',' ','I',
		'n','s','t','r','.',' ',' ',' '
};
const unsigned char  _mt_prime_reset[16] __attribute__ ((section(".rodata"))) = {
		'R','e','s','e','t',' ','S','y',
		's','t','e','m','u',' ',' ',' '
};
const unsigned char  _mt_choose_inst[16] __attribute__ ((section(".rodata"))) = {
		'W','y','b','i','e','r','z',' ',
		'I','n','s','t','r','.',' ',' '
};
const unsigned char  _mt_choose_params[16] __attribute__ ((section(".rodata"))) = {
		'P','a','r','.','W','a','r','t',
		'.','J','e','d','.',' ',' ',' '
};
const unsigned char  _mt_waitexec_top[16] __attribute__ ((section(".rodata"))) = {
		'T','r','w','a',' ','w','y','k',
		'o','n','y','w','a','n','i','e'
};
const unsigned char  _mt_waitexec_bot[16] __attribute__ ((section(".rodata"))) = {
		'p','r','o','g','r','a','m','u',
		' ',' ',' ',' ',' ',' ',' ',' '
};
const unsigned char  _mt_error_top[16] __attribute__ ((section(".rodata"))) = {
		'W','y','s','t','a','p','i','l',
		' ','B','l','a','d',':',' ',' '
};
const unsigned char  _mt_prog_main[16] __attribute__ ((section(".rodata"))) = {
		'M','e','n','u',' ','p','r','o',
		'g','r','a','m','o','w',' ',' '
};
const unsigned char  _mt_prog_new[16] __attribute__ ((section(".rodata"))) = {
		'N','o','w','y',' ','p','r','o',
		'g','r','a','m',' ',' ',' ',' '
};
const unsigned char  _mt_prog_edit[16] __attribute__ ((section(".rodata"))) = {
		'E','d','y','t','u','j',' ','p',
		'r','o','g','r','a','m',' ',' '
};
const unsigned char  _mt_prog_exec[16] __attribute__ ((section(".rodata"))) = {
		'U','r','u','c','h','o','m',' ',
		'p','r','o','g','r','a','m',' '
};
const unsigned char  _mt_prime_program[16] __attribute__ ((section(".rodata"))) = {
		'M','e','n','u',' ','p','r','o',
		'g','r','a','m','o','w',' ',' '
};
const unsigned char  _mt_prog_choose[16] __attribute__ ((section(".rodata"))) = {
		'W','y','b','e','r','z',' ',' ',
		'p','r','o','g','r','a','m',' '
};
const unsigned char  _mt_prog_newname[16] __attribute__ ((section(".rodata"))) = {
		'W','p','r','o','w','a','d','z',
		' ','n','a','z','w','e',' ',' '
};
const unsigned char  _mt_prog_nameexists[16] __attribute__ ((section(".rodata"))) = {
		'N','a','z','w','a',' ','j','u',
		'z',' ','z','a','j','e','t','a'
};
const unsigned char  _mt_prog_inst_edittop[16] __attribute__ ((section(".rodata"))) = {
		'W','y','b','e','r','z',' ','a',
		'k','c','j','e',' ',' ',' ',' '
};
const unsigned char  _mt_prog_inst_editremove[16] __attribute__ ((section(".rodata"))) = {
		'U','s','u','n',' ',' ',' ',' ',
		' ',' ',' ',' ',' ',' ',' ',' '
};
const unsigned char  _mt_prog_inst_editchange[16] __attribute__ ((section(".rodata"))) = {
		'Z','m','i','e','n',' ',' ',' ',
		' ',' ',' ',' ',' ',' ',' ',' '
};

const unsigned char* const _menu_texts_[] __attribute__ ((section(".rodata"))) =
{
		_mt_prime_topmenu,
		_mt_prime_singleinst,
		_mt_prime_reset,
		_mt_choose_inst,
		_mt_choose_params,
		_mt_waitexec_top,
		_mt_waitexec_bot,
		_mt_error_top,
		_mt_prog_main,
		_mt_prog_new,
		_mt_prog_edit,
		_mt_prog_exec,
		_mt_prime_program,
		_mt_prog_choose,
		_mt_prog_newname,
		_mt_prog_inst_edittop,
		_mt_prog_inst_editremove,
		_mt_prog_inst_editchange
};
