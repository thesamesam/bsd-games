// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#pragma once

struct func_tab {
    char f_char;
    int (*f_funct) (void);
};

extern const struct func_tab cmdlist[];

struct ext_func_tab {
    const char *ef_txt;
    int (*ef_funct) (void);
};

extern const struct ext_func_tab extcmdlist[];
