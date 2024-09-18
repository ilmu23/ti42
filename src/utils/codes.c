/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ivalimak <ivalimak@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 21:50:11 by ivalimak          #+#    #+#             */
/*   Updated: 2024/09/18 13:02:32 by ivalimak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_internal/ti42_internal.h"

const speed_t	speeds[SPEEDCOUNT * 2] __UNUSED = {0, 1, 2, 3, 4,
	5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 4097, 4098, 4099, 4100, 4101, 4102,
	4103, 4104, 4105, 4106, 4107, 4108, 4109, 4110, 4111, 0, 50, 75, 110, 134, 150,
	200, 300, 600, 1200, 1800, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400,
	460800, 500000, 576000, 921600, 1000000, 1152000, 1500000, 2000000, 2500000,
	3000000, 3500000, 4000000};

const char	*tidirs[4] __UNUSED = {"/etc/terminfo", "/lib/terminfo", "/usr/share/terminfo", NULL};

const char	*boolcodes[BOOLCOUNT] __UNUSED = {"bw", "am",
	"xsb", "xhp", "xenl", "eo", "gn", "hc", "km", "hs", "in", "da", "db", "mir",
	"msgr", "os", "eslok", "xt", "hz", "ul", "xon", "nxon", "mc5i", "chts", "nrrmc",
	"npc", "ndscr", "ccc", "bce", "hls", "xhpa", "crxm", "daisy", "xvpa", "sam",
	"cpix", "lpix", "OTbs", "OTns", "OTnc", "OTMT", "OTNL", "OTpt", "OTXR"};

const char	*numcodes[NUMCOUNT] __UNUSED = {"cols", "it",
	"lines", "lm", "xmc", "pb", "vt", "wsl", "nlab", "lh", "lw", "ma", "wnum",
	"colors", "pairs", "ncv", "bufsz", "spinv", "spinh", "maddr", "mjump", "mcs",
	"mls", "npins", "orc", "orl", "orhi", "orvi", "cps", "widcs", "btns", "bitwin",
	"bitype", "OTug", "OTdC", "OTdN", "OTdB", "OTdT", "OTkn"};

const char	*strcodes[STRCOUNT] __UNUSED = {"cbt", "bel",
	"cr", "csr", "tbc", "clear", "el", "ed", "hpa", "cmdch", "cup", "cud1", "home",
	"civis", "cub1", "mrcup", "cnorm", "cuf1", "ll", "cuu1", "cvvis", "dch1",
	"dl1", "dsl", "hd", "smacs", "blink", "bold", "smcup", "smdc", "dim", "smir",
	"invis", "prot", "rev", "smso", "smul", "ech", "rmacs", "sgr0", "rmcup", "rmdc",
	"rmir", "rmso", "rmul", "flash", "ff", "fsl", "is1", "is2", "is3", "if", "ich1",
	"il1", "ip", "kbs", "ktbc", "kclr", "kctab", "kdch1", "kdl1", "kcud1", "krmir",
	"kel", "ked", "kf0", "kf1", "kf10", "kf2", "kf3", "kf4", "kf5", "kf6", "kf7", "kf8",
	"kf9", "khome", "kich1", "kil1", "kcub1", "kll", "knp", "kpp", "kcuf1", "kind",
	"kri", "khts", "kcuu1", "rmkx", "smkx", "lf0", "lf1", "lf10", "lf2", "lf3",
	"lf4", "lf5", "lf6", "lf7", "lf8", "lf9", "rmm", "smm", "nel", "pad", "dch",
	"dl", "cud", "ich", "indn", "il", "cub", "cuf", "rin", "cuu", "pfkey", "pfloc",
	"pfx", "mc0", "mc4", "mc5", "rep", "rs1", "rs2", "rs3", "rf", "rc", "vpa", "sc",
	"ind", "ri", "sgr", "hts", "wind", "ht", "tsl", "uc", "hu", "iprog", "ka1",
	"ka3", "kb2", "kc1", "kc3", "mc5p", "rmp", "acsc", "pln", "kcbt", "smxon",
	"rmxon", "smam", "rmam", "xonc", "xoffc", "enacs", "smln","rmln", "kbeg",
	"kcan", "kclo", "kcmd", "kcpy", "kcrt", "kend", "kent", "kext", "kfnd",
	"khlp", "kmrk", "kmsg", "kmov", "knxt", "kopn", "kopt", "kprv", "kprt",
	"krdo", "kref", "krfr", "krpl", "krst", "kres", "ksav", "kspd", "kund",
	"kBEG", "kCAN", "kCMD", "kCPY", "kCRT", "kDC", "kDL", "kslt", "kEND", "kEOL",
	"kEXT", "kFND", "kHLP", "kHOM", "kIC", "kLFT", "kMSG", "kMOV", "kNXT", "kOPT",
	"kPRV", "kPRT", "kRDO", "kRPL", "kRIT", "kRES", "kSAV", "kSPD", "kUND", "rfi",
	"kf11", "kf12", "kf13", "kf14", "kf15", "kf16", "kf17", "kf18", "kf19", "kf20",
	"kf21", "kf22", "kf23", "kf24", "kf25", "kf26", "kf27", "kf28", "kf29", "kf30",
	"kf31", "kf32", "kf33", "kf34", "kf35", "kf36", "kf37", "kf38", "kf39", "kf40",
	"kf41", "kf42", "kf43", "kf44", "kf45", "kf46", "kf47", "kf48", "kf49", "kf50",
	"kf51", "kf52", "kf53", "kf54", "kf55", "kf56", "kf57", "kf58", "kf59", "kf60",
	"kf61", "kf62", "kf63", "el1", "mgc", "smgl", "smgr", "fln", "sclk", "dclk",
	"rmclk", "cwin", "wingo", "hup", "dial", "qdial", "tone", "pulse", "hook",
	"pause", "wait", "u0", "u1", "u2", "u3", "u4", "u5", "u6", "u7", "u8", "u9",
	"op", "oc", "initc", "initp", "scp", "setf", "setb", "cpi", "lpi", "chr", "cvr",
	"defc", "swidm", "sdrfq", "sitm", "slm", "smicm", "snlq", "snrmq", "sshm",
	"ssubm", "ssupm", "sum", "rwidm", "ritm", "rlm", "rmicm", "rshm", "rsubm",
	"rsupm", "rum", "mhpa", "mcud1", "mcub1", "mcuf1", "mvpa", "mcuu1", "porder",
	"mcud", "mcub", "mcuf", "mcuu", "scs", "smgb", "smgbp","smglp", "smgrp", "smgt",
	"smgtp", "sbim", "scsd", "rbim", "rcsd", "subcs", "supcs", "docr", "zerom",
	"csnm", "kmous", "minfo", "reqmp", "getm", "setaf", "setab", "pfxl", "devt",
	"csin", "s0ds", "s1ds", "s2ds", "s3ds", "smglr", "smgtb", "birep", "binel",
	"bicr", "colornm", "defbi", "endbi", "setcolor", "slines", "dispc", "smpch",
	"rmpch", "smsc", "rmsc", "pctrm", "scesc", "scesa", "ehhlm", "elhlm", "elohlm",
	"erhlm", "ethlm", "evhlm", "sgr1", "slength", "OTi2", "OTrs", "OTnl", "OTbc",
	"OTko", "OTma", "OTG2", "OTG3", "OTG1", "OTG4", "OTGR", "OTGL", "OTGU", "OTGD",
	"OTGH", "OTGV", "OTGC", "meml", "memu", "box1",};
