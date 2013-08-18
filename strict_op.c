#include "php.h"
#include "php_strict.h"
#include "source_graph.h"
#include "strict_op.h"

#define STRICT_IGNORE  0
#define STRICT_USE_OP1 1
#define STRICT_USE_OP2 2
#define STRICT_USE_ALL 3

/* {{{ static const strict_op_info op_info[] */
static const strict_op_info op_info[] = {
		/*   0 */ {STRICT_IGNORE,  "ZEND_NOP"                            },
		/*   1 */ {STRICT_USE_ALL, "ZEND_ADD"                            },
		/*   2 */ {STRICT_USE_ALL, "ZEND_SUB"                            },
		/*   3 */ {STRICT_USE_ALL, "ZEND_MUL"                            },
		/*   4 */ {STRICT_USE_ALL, "ZEND_DIV"                            },
		/*   5 */ {STRICT_USE_ALL, "ZEND_MOD"                            },
		/*   6 */ {STRICT_USE_ALL, "ZEND_SL"                             },
		/*   7 */ {STRICT_USE_ALL, "ZEND_SR"                             },
		/*   8 */ {STRICT_USE_ALL, "ZEND_CONCAT"                         },
		/*   9 */ {STRICT_USE_ALL, "ZEND_BW_OR"                          },
		/*  10 */ {STRICT_USE_ALL, "ZEND_BW_AND"                         },
		/*  11 */ {STRICT_USE_ALL, "ZEND_BW_XOR"                         },
		/*  12 */ {STRICT_USE_ALL, "ZEND_BW_NOT"                         },
		/*  13 */ {STRICT_USE_ALL, "ZEND_BOOL_NOT"                       },
		/*  14 */ {STRICT_USE_ALL, "ZEND_BOOL_XOR"                       },
		/*  15 */ {STRICT_USE_ALL, "ZEND_IS_IDENTICAL"                   },
		/*  16 */ {STRICT_USE_ALL, "ZEND_IS_NOT_IDENTICAL"               },
		/*  17 */ {STRICT_USE_ALL, "ZEND_IS_EQUAL"                       },
		/*  18 */ {STRICT_USE_ALL, "ZEND_IS_NOT_EQUAL"                   },
		/*  19 */ {STRICT_USE_ALL, "ZEND_IS_SMALLER"                     },
		/*  20 */ {STRICT_USE_ALL, "ZEND_IS_SMALLER_OR_EQUAL"            },
		/*  21 */ {STRICT_USE_OP1, "ZEND_CAST"                           },
		/*  22 */ {STRICT_IGNORE,  "ZEND_QM_ASSIGN"                      },
		/*  23 */ {STRICT_USE_OP1, "ZEND_ASSIGN_ADD"                     },
		/*  24 */ {STRICT_USE_OP1, "ZEND_ASSIGN_SUB"                     },
		/*  25 */ {STRICT_USE_OP1, "ZEND_ASSIGN_MUL"                     },
		/*  26 */ {STRICT_USE_OP1, "ZEND_ASSIGN_DIV"                     },
		/*  27 */ {STRICT_USE_OP1, "ZEND_ASSIGN_MOD"                     },
		/*  28 */ {STRICT_USE_OP1, "ZEND_ASSIGN_SL"                      },
		/*  29 */ {STRICT_USE_OP1, "ZEND_ASSIGN_SR"                      },
		/*  30 */ {STRICT_USE_OP1, "ZEND_ASSIGN_CONCAT"                  },
		/*  31 */ {STRICT_USE_OP1, "ZEND_ASSIGN_BW_OR"                   },
		/*  32 */ {STRICT_USE_OP1, "ZEND_ASSIGN_BW_AND"                  },
		/*  33 */ {STRICT_USE_OP1, "ZEND_ASSIGN_BW_XOR"                  },
		/*  34 */ {STRICT_USE_OP1, "ZEND_PRE_INC"                        },
		/*  35 */ {STRICT_USE_OP1, "ZEND_PRE_DEC"                        },
		/*  36 */ {STRICT_USE_OP1, "ZEND_POST_INC"                       },
		/*  37 */ {STRICT_USE_OP1, "ZEND_POST_DEC"                       },
		/*  38 */ {STRICT_USE_OP2, "ZEND_ASSIGN"                         },
		/*  39 */ {STRICT_USE_OP2, "ZEND_ASSIGN_REF"                     },
		/*  40 */ {STRICT_USE_OP1, "ZEND_ECHO"                           },
		/*  41 */ {STRICT_USE_OP1, "ZEND_PRINT"                          },
		/*  42 */ {STRICT_IGNORE,  "ZEND_JMP"                            },
		/*  43 */ {STRICT_IGNORE,  "ZEND_JMPZ"                           },
		/*  44 */ {STRICT_IGNORE,  "ZEND_JMPNZ"                          },
		/*  45 */ {STRICT_IGNORE,  "ZEND_JMPZNZ"                         },
		/*  46 */ {STRICT_USE_ALL, "ZEND_JMPZ_EX"                        },
		/*  47 */ {STRICT_IGNORE,  "ZEND_JMPNZ_EX"                       },
		/*  48 */ {STRICT_USE_ALL, "ZEND_CASE"                           },
		/*  49 */ {STRICT_IGNORE,  "ZEND_SWITCH_FREE"                    },
		/*  50 */ {STRICT_IGNORE,  "ZEND_BRK"                            },
		/*  51 */ {STRICT_IGNORE,  "ZEND_CONT"                           },
		/*  52 */ {STRICT_USE_OP1, "ZEND_BOOL"                           },
		/*  53 */ {STRICT_IGNORE,  "ZEND_INIT_STRING"                    },
		/*  54 */ {STRICT_IGNORE,  "ZEND_ADD_CHAR"                       },
		/*  55 */ {STRICT_IGNORE,  "ZEND_ADD_STRING"                     },
		/*  56 */ {STRICT_USE_OP2, "ZEND_ADD_VAR"                        },
		/*  57 */ {STRICT_IGNORE,  "ZEND_BEGIN_SILENCE"                  },
		/*  58 */ {STRICT_IGNORE,  "ZEND_END_SILENCE"                    },
		/*  59 */ {STRICT_USE_OP2, "ZEND_INIT_FCALL_BY_NAME"             },
		/*  60 */ {STRICT_IGNORE,  "ZEND_DO_FCALL"                       },
		/*  61 */ {STRICT_IGNORE,  "ZEND_DO_FCALL_BY_NAME"               },
		/*  62 */ {STRICT_USE_OP1, "ZEND_RETURN"                         },
		/*  63 */ {STRICT_IGNORE,  "ZEND_RECV"                           },
		/*  64 */ {STRICT_IGNORE,  "ZEND_RECV_INIT"                      },
		/*  65 */ {STRICT_IGNORE,  "ZEND_SEND_VAL"                       },
		/*  66 */ {STRICT_USE_OP1, "ZEND_SEND_VAR"                       },
		/*  67 */ {STRICT_IGNORE,  "ZEND_SEND_REF"                       },
		/*  68 */ {STRICT_IGNORE,  "ZEND_NEW"                            },

		/*  69 */ {STRICT_USE_ALL, "ZEND_INIT_NS_FCALL_BY_NAME"          },
		/*  70 */ {STRICT_USE_ALL, "ZEND_FREE"                           },
		/*  71 */ {STRICT_IGNORE,  "ZEND_INIT_ARRAY"                     },
		/*  72 */ {STRICT_USE_ALL, "ZEND_ADD_ARRAY_ELEMENT"              },
		/*  73 */ {STRICT_USE_ALL, "ZEND_INCLUDE_OR_EVAL"                },
		/*  74 */ {STRICT_IGNORE,  "ZEND_UNSET_VAR"                      },
		/*  75 */ {STRICT_IGNORE,  "ZEND_UNSET_DIM"                      },
		/*  76 */ {STRICT_IGNORE,  "ZEND_UNSET_OBJ"                      },
		/*  77 */ {STRICT_USE_ALL, "ZEND_FE_RESET"                       },
		/*  78 */ {STRICT_USE_ALL, "ZEND_FE_FETCH"                       },
		/*  79 */ {STRICT_USE_ALL, "ZEND_EXIT"                           },
		/*  80 */ {STRICT_USE_ALL, "ZEND_FETCH_R"                        },
		/*  81 */ {STRICT_USE_ALL, "ZEND_FETCH_DIM_R"                    },
		/*  82 */ {STRICT_USE_ALL, "ZEND_FETCH_OBJ_R"                    },
		/*  83 */ {STRICT_USE_ALL, "ZEND_FETCH_W"                        },
		/*  84 */ {STRICT_USE_ALL, "ZEND_FETCH_DIM_W"                    },
		/*  85 */ {STRICT_USE_ALL, "ZEND_FETCH_OBJ_W"                    },
		/*  86 */ {STRICT_USE_ALL, "ZEND_FETCH_RW"                       },
		/*  87 */ {STRICT_USE_ALL, "ZEND_FETCH_DIM_RW"                   },
		/*  88 */ {STRICT_USE_ALL, "ZEND_FETCH_OBJ_RW"                   },
		/*  89 */ {STRICT_USE_ALL, "ZEND_FETCH_IS"                       },
		/*  90 */ {STRICT_USE_ALL, "ZEND_FETCH_DIM_IS"                   },
		/*  91 */ {STRICT_USE_ALL, "ZEND_FETCH_OBJ_IS"                   },
		/*  92 */ {STRICT_USE_ALL, "ZEND_FETCH_FUNC_ARG"                 },
		/*  93 */ {STRICT_USE_ALL, "ZEND_FETCH_DIM_FUNC_ARG"             },
		/*  94 */ {STRICT_USE_ALL, "ZEND_FETCH_OBJ_FUNC_ARG"             },
		/*  95 */ {STRICT_USE_ALL, "ZEND_FETCH_UNSET"                    },
		/*  96 */ {STRICT_USE_ALL, "ZEND_FETCH_DIM_UNSET"                },
		/*  97 */ {STRICT_USE_ALL, "ZEND_FETCH_OBJ_UNSET"                },
		/*  98 */ {STRICT_USE_ALL, "ZEND_FETCH_DIM_TMP_VAR"              },
		/*  99 */ {STRICT_USE_ALL, "ZEND_FETCH_CONSTANT"                 },
		/* 100 */ {STRICT_USE_ALL, "ZEND_GOTO"                           },
		/* 101 */ {STRICT_USE_ALL, "ZEND_EXT_STMT"                       },
		/* 102 */ {STRICT_USE_ALL, "ZEND_EXT_FCALL_BEGIN"                },
		/* 103 */ {STRICT_USE_ALL, "ZEND_EXT_FCALL_END"                  },
		/* 104 */ {STRICT_USE_ALL, "ZEND_EXT_NOP"                        },
		/* 105 */ {STRICT_USE_ALL, "ZEND_TICKS"                          },
		/* 106 */ {STRICT_USE_ALL, "ZEND_SEND_VAR_NO_REF"                },
		/* 107 */ {STRICT_USE_ALL, "ZEND_CATCH"                          },
		/* 108 */ {STRICT_USE_ALL, "ZEND_THROW"                          },
		/* 109 */ {STRICT_USE_ALL, "ZEND_FETCH_CLASS"                    },
		/* 110 */ {STRICT_USE_ALL, "ZEND_CLONE"                          },
		/* 111 */ {STRICT_USE_ALL, "ZEND_RETURN_BY_REF"                  },
		/* 112 */ {STRICT_USE_ALL, "ZEND_INIT_METHOD_CALL"               },
		/* 113 */ {STRICT_USE_ALL, "ZEND_INIT_STATIC_METHOD_CALL"        },
		/* 114 */ {STRICT_IGNORE,  "ZEND_ISSET_ISEMPTY_VAR"              },
		/* 115 */ {STRICT_USE_ALL, "ZEND_ISSET_ISEMPTY_DIM_OBJ"          },
		/* 116 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 117 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 118 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 119 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 120 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 121 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 122 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 123 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 124 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 125 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 126 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 127 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 128 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 129 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 130 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 131 */ {STRICT_IGNORE,  "UNKNOWN"                             },
		/* 132 */ {STRICT_USE_ALL, "ZEND_PRE_INC_OBJ"                    },
		/* 133 */ {STRICT_USE_ALL, "ZEND_PRE_DEC_OBJ"                    },
		/* 134 */ {STRICT_USE_ALL, "ZEND_POST_INC_OBJ"                   },
		/* 135 */ {STRICT_USE_ALL, "ZEND_POST_DEC_OBJ"                   },
		/* 136 */ {STRICT_USE_ALL, "ZEND_ASSIGN_OBJ"                     },
		/* 137 */ {STRICT_USE_ALL, "ZEND_OP_DATA"                        },
		/* 138 */ {STRICT_USE_ALL, "ZEND_INSTANCEOF"                     },
		/* 139 */ {STRICT_USE_ALL, "ZEND_DECLARE_CLASS"                  },
		/* 140 */ {STRICT_USE_ALL, "ZEND_DECLARE_INHERITED_CLASS"        },
		/* 141 */ {STRICT_USE_ALL, "ZEND_DECLARE_FUNCTION"               },
		/* 142 */ {STRICT_USE_ALL, "ZEND_RAISE_ABSTRACT_ERROR"           },
		/* 143 */ {STRICT_USE_ALL, "ZEND_DECLARE_CONST"                  },
		/* 144 */ {STRICT_USE_ALL, "ZEND_ADD_INTERFACE"                  },
		/* 145 */ {STRICT_USE_ALL, "ZEND_DECLARE_INHERITED_CLASS_DELAYED"},
		/* 146 */ {STRICT_USE_ALL, "ZEND_VERIFY_ABSTRACT_CLASS"          },
		/* 147 */ {STRICT_USE_ALL, "ZEND_ASSIGN_DIM"                     },
		/* 148 */ {STRICT_USE_ALL, "ZEND_ISSET_ISEMPTY_PROP_OBJ"         },
		/* 149 */ {STRICT_USE_ALL, "ZEND_HANDLE_EXCEPTION"               },
		/* 150 */ {STRICT_USE_ALL, "ZEND_USER_OPCODE"                    },
		/* 151 */ {STRICT_IGNORE , "UNKNOWN"                             },
		/* 152 */ {STRICT_USE_ALL, "ZEND_JMP_SET"                        },
		/* 153 */ {STRICT_USE_ALL, "ZEND_DECLARE_LAMBDA_FUNCTION"        },
		/* 154 */ {STRICT_USE_ALL, "ZEND_ADD_TRAIT"                      },
		/* 155 */ {STRICT_USE_ALL, "ZEND_BIND_TRAITS"                    },
		/* 156 */ {STRICT_USE_ALL, "ZEND_SEPARATE"                       },
		/* 157 */ {STRICT_USE_ALL, "ZEND_QM_ASSIGN_VAR"                  },
		/* 158 */ {STRICT_USE_ALL, "ZEND_JMP_SET_VAR"                    },
		/* 159 */ {STRICT_USE_ALL, "ZEND_DISCARD_EXCEPTION"              },
		/* 160 */ {STRICT_USE_ALL, "ZEND_YIELD"                          },
		/* 161 */ {STRICT_USE_ALL, "ZEND_GENERATOR_RETURN"               },
		/* 162 */ {STRICT_USE_ALL, "ZEND_FAST_CALL"                      },
		/* 163 */ {STRICT_USE_ALL, "ZEND_FAST_RET"                       },
};
/* }}} */
#define MAX_OP_CODE 163


/* {{{ assigned_map */
typedef struct _assigned_map {
	unsigned int size;
	int *current;
	int stack_pos;
	struct {
		int position;
		int* map;
	} stack[100];
} assigned_map;

assigned_map *create_assigned_map(unsigned int size) {
	assigned_map *am = malloc(sizeof(assigned_map));
	am->size = size;
	am->current = calloc(sizeof(int), size);
	am->stack_pos = 0;
	return am;
}

void free_assigned_map(assigned_map *am) {
	while(0 <= pop_assigned_map(am));
	free(am->current);
	free(am);
}

void stack_assigned_map(assigned_map *am, int position) {
	int *clone = malloc(sizeof(int) * am->size);
	memcpy(clone, am->current, sizeof(int) * am->size);

	am->stack[am->stack_pos].map = am->current;
	am->stack[am->stack_pos].position = position;
	++am->stack_pos;
	am->current = clone;
}

int pop_assigned_map(assigned_map *am) {
	if (am->stack_pos <= 0) {
		return -1;
	}

	free(am->current);

	--am->stack_pos;
	am->current = am->stack[am->stack_pos].map;
	return am->stack[am->stack_pos].position;
}

int get_stacked_position(assigned_map *am) {
	return (0 < am->stack_pos) ?
		am->stack[am->stack_pos-1].position:
		-1
	;
}

void print_current_map(assigned_map *am) {
	int i;
	strict_verbose("\n  Current Map: (");
	for (i=0; i<am->size;++i) {
		strict_verbose("%s%d:%d", i==0?"":", " ,i, am->current[i]);
	}
	strict_verbose(")\n");
}

void assign_map(assigned_map *am, unsigned int var) {
	assert(var < am->size);
	am->current[var] = 1;
}

int is_assigned(assigned_map *am, unsigned int var) {
	assert(var < am->size);
	return am->current[var];
}
/* }}} */



static int detect_assignment(assigned_map *am, zend_op_array *opa, int position)
{
	zend_uint var;
	int detect = 0;
	const zend_op op = opa->opcodes[position];

	if (op.opcode == ZEND_ASSIGN) {
		if (op.STRICT_TYPE(op1) == IS_CV) {
			var =  op.STRICT_ZNODE_ELEM(op1, var);
			detect = 1;
		}
	} else if (op.opcode == ZEND_RECV || op.opcode == ZEND_RECV_INIT) {
		var = op.STRICT_ZNODE_ELEM(result, var);
		detect = 1;
	} else if (op.opcode == ZEND_SEND_REF) {
		/* 必ずしも割り当てられるとは限らないが、未定義ではなくなる */
		if (op.STRICT_TYPE(op1) == IS_CV) {
			var =  op.STRICT_ZNODE_ELEM(op1, var);
			detect = 1;
		}
	}

	if (detect) {
		assign_map(am, var);

		const char* name = opa->vars[var].name;
		strict_verbose("  FOUND ASSIGNMENT %s (%d) line %d", name, var, op.lineno);
	}

	return detect;
}

static void strict_error(int type, const char *filename, const int line, const char *format, ...) {
	va_list args;
	va_start(args, format);
	zend_error_cb(type, filename, line, format, args);
	va_end(args);
}

static int detect_use_of_unassigned_variable(assigned_map *am, zend_op_array *opa, int position)
{
	const zend_op op = opa->opcodes[position];
	int detect = 0;

	if (MAX_OP_CODE < op.opcode) {
		return 0;
	}

	if ((op_info[op.opcode].use_flag & STRICT_USE_OP1) && op.STRICT_TYPE(op1) == IS_CV) {
		if (!is_assigned(am, op.STRICT_ZNODE_ELEM(op1, var))) {
			const char* name = opa->vars[op.STRICT_ZNODE_ELEM(op1, var)].name;
			strict_error(E_COMPILE_WARNING, STRICT_G(filename), op.lineno,
				"Use of unassigned local variable $%s.", name
			);
			detect = 1;
		}
	}

	if ((op_info[op.opcode].use_flag & STRICT_USE_OP2) && op.STRICT_TYPE(op2) == IS_CV) {
		if (!is_assigned(am, op.STRICT_ZNODE_ELEM(op2, var))) {
			const char* name = opa->vars[op.STRICT_ZNODE_ELEM(op2, var)].name;
			strict_error(E_COMPILE_WARNING, STRICT_G(filename), op.lineno,
				"Use of unassigned local variable $%s.", name
			);
			detect = 1;
		}
	}

	if (detect) {
		STRICT_G(detect) = 1;
	}

	return detect;
}

static zend_brk_cont_element* strict_find_brk_cont(zend_uint nest_levels, int array_offset, zend_op_array *op_array)
{
	zend_brk_cont_element *jmp_to;

	do {
			jmp_to = &op_array->brk_cont_array[array_offset];
			array_offset = jmp_to->parent;
	} while (--nest_levels > 0);
	return jmp_to;
}

/*
static int strict_detect_jump(zend_op_array *opa, zend_uint position, zend_op **jmp1, zend_op **jmp2)
{
	const zend_op op = opa->opcodes[position];

	if (op.opcode == ZEND_JMP) {
		*jmp1 = STRICT_ZNODE_ELEM(op.op1, jmp_addr);
		return 1;
	} else if (op.opcode == ZEND_JMPZ || op.opcode == ZEND_JMPNZ ||
				op.opcode == ZEND_JMPZ_EX || op.opcode == ZEND_JMPNZ_EX) {
		*jmp1 = &opa->opcodes[position + 1];
		*jmp2 = STRICT_ZNODE_ELEM(op.op2, jmp_addr);
		return 1;
	} else if (op.opcode == ZEND_JMPZNZ) {
		*jmp1 = &opa->opcodes[STRICT_ZNODE_ELEM(op.op2, opline_num)];
		*jmp2 = &opa->opcodes[op.extended_value];
		return 1;
	} else if (op.opcode == ZEND_BRK || op.opcode == ZEND_CONT) {
		zend_brk_cont_element *el;

		if (STRICT_TYPE(op.op2) == IS_CONST
			&& STRICT_ZNODE_ELEM(op.op1, jmp_addr) != (zend_op*) 0xFFFFFFFF
		) {
			zend_brk_cont_element *el;
#if PHP_VERSION_ID >= 50399
			el = strict_find_brk_cont(op.op2.constant, STRICT_ZNODE_ELEM(op.op1, opline_num), opa);
#else
			el = strict_find_brk_cont(op.op2.u.constant.value.lval, STRICT_ZNODE_ELEM(op.op1, opline_num), opa);
#endif
			*jmp1 = &opa->opcodes[op.opcode == ZEND_BRK ? el->brk : el->cont];
			*jmp2 = NULL;
			return 1;
		}
	} else if (op.opcode == ZEND_FE_RESET || op.opcode == ZEND_FE_FETCH) {
		*jmp1 = &opa->opcodes[position + 1];;
		*jmp2 = &opa->opcodes[STRICT_ZNODE_ELEM(op.op2, opline_num)];
		return 1;
#if PHP_VERSION_ID >= 50300
	} else if (op.opcode == ZEND_GOTO) {
		*jmp1 = STRICT_ZNODE_ELEM(op.op1, jmp_addr);
		*jmp2 = NULL;
		return 1;
#endif
	}

	return 0;
}
*/

static int detect_jump_pos(zend_op_array *opa, zend_uint position, int *jmp1, int *jmp2)
{
 	zend_op *base_address = &(opa->opcodes[0]);

	zend_op opcode = opa->opcodes[position];
	if (opcode.opcode == ZEND_JMP) {
		*jmp1 = ((long) STRICT_ZNODE_ELEM(opcode.op1, jmp_addr) - (long) base_address) / sizeof(zend_op);
		return 1;
	} else if (
		opcode.opcode == ZEND_JMPZ ||
		opcode.opcode == ZEND_JMPNZ ||
		opcode.opcode == ZEND_JMPZ_EX ||
		opcode.opcode == ZEND_JMPNZ_EX
	) {
		*jmp1 = position + 1;
		*jmp2 = ((long) STRICT_ZNODE_ELEM(opcode.op2, jmp_addr) - (long) base_address) / sizeof(zend_op);
		return 1;
	} else if (opcode.opcode == ZEND_JMPZNZ) {
		*jmp1 = STRICT_ZNODE_ELEM(opcode.op2, opline_num);
		*jmp2 = opcode.extended_value;
		return 1;
	} else if (opcode.opcode == ZEND_BRK || opcode.opcode == ZEND_CONT) {
		zend_brk_cont_element *el;

		if (STRICT_TYPE(opcode.op2) == IS_CONST
			&& STRICT_ZNODE_ELEM(opcode.op1, jmp_addr) != (zend_op*) 0xFFFFFFFF ) {
#if PHP_VERSION_ID >= 50399
			el = strict_find_brk_cont(opcode.op2.constant, STRICT_ZNODE_ELEM(opcode.op1, opline_num), opa);
#else
			el = strict_find_brk_cont(opcode.op2.u.constant.value.lval, STRICT_ZNODE_ELEM(opcode.op1, opline_num), opa);
#endif
			*jmp1 = opcode.opcode == ZEND_BRK ? el->brk : el->cont;
			return 1;
		}
	} else if (opcode.opcode == ZEND_FE_RESET || opcode.opcode == ZEND_FE_FETCH) {
		*jmp1 = position + 1;
		*jmp2 = STRICT_ZNODE_ELEM(opcode.op2, opline_num);
		return 1;
#if PHP_VERSION_ID >= 50300
	} else if (opcode.opcode == ZEND_GOTO) {
		*jmp1 = ((long) STRICT_ZNODE_ELEM(opcode.op1, jmp_addr) - (long) base_address) / sizeof(zend_op);
		return 1;
#endif
	}
	return 0;
}

/* {{{ void strict_scan_op_array(zend_op_array op_array) */
void strict_scan_op_array(zend_op_array *opa TSRMLS_DC)
{
	int position;
	int* passed;
	assigned_map *am;

	am = create_assigned_map(opa->last_var+1);
	passed = calloc(opa->last+1, sizeof(int));

	position = 0;
	while (position < opa->last) {
		int jmp1 = -1, jmp2 = -1;

		strict_verbose("%4d:", position);
		detect_use_of_unassigned_variable(am, opa, position);
		detect_assignment(am, opa, position);

		passed[position] = 1;
		if (detect_jump_pos(opa, position, &jmp1, &jmp2)) {
			if (jmp1 != -1 && jmp2 != -1) {
				position = MIN(jmp1, jmp2);
				stack_assigned_map(am, MAX(jmp1, jmp2));
				strict_verbose("  STACK %d and jump to %d", MAX(jmp1, jmp2), position);
			} else { // jmp1 only
				if (passed[jmp1]) {
					if (get_stacked_position(am) < 0) {
						position += 1;
						strict_verbose("  jump but passed %d, next %d", jmp1, position);
					} else {
						position = pop_assigned_map(am);
						strict_verbose("  POP and jump to %d", position);
					}
				} else if (0 < get_stacked_position(am) && get_stacked_position(am) < jmp1) {
					position = pop_assigned_map(am);
					strict_verbose("  POP and jump to %d (overide %d)", position, jmp1);
				} else {
					position = jmp1;
					strict_verbose("  jump to %d", position);
				}
			}
		} else {
			position += 1;
		}

		//strict_verbose("stacked_position = %d\n", get_stacked_position(am));
		if (get_stacked_position(am) == position) {
			pop_assigned_map(am);
			strict_verbose("  POP and next %d", position);
			print_current_map(am);
		}

		strict_verbose("\n");
	}

	free(passed);
}
/* }}} */



static inline const char* get_op_type_name(int type) {
	static const char* op_type_names[] = {
		"CONST  ",
		"TMP_VAR",
		"VAR    ",
		"UNUSED ",
		"CV     ",
		"UNKNOWN",
	};

	switch (type) {
		case IS_CONST:   return op_type_names[0];
		case IS_TMP_VAR: return op_type_names[1];
		case IS_VAR:     return op_type_names[2];
		case IS_UNUSED:  return op_type_names[3];
		case IS_CV:      return op_type_names[4];
		default:         return op_type_names[5];
	}
}


/* {{{ void strict_op_dump(const zend_op op) */
void strict_op_dump(zend_op_array* opa, int position TSRMLS_DC) {

	zend_op op = opa->opcodes[position];
	int jmp1 = -1, jmp2 = -1;

	php_printf(
		"D%4d: %3d=%-16s\t"
		" ev:%-2d, ln:%-2d"
		" 1:%s(%2d), 2:%s(%2d), r:%s(%2d)",
		position, op.opcode, op_info[op.opcode].name,
		op.extended_value, op.lineno,
		get_op_type_name(op.op1.op_type), (op.op1.op_type==IS_CV ? op.op1.u.var : -1),
		get_op_type_name(op.op2.op_type), (op.op2.op_type==IS_CV ? op.op2.u.var : -1),
		get_op_type_name(op.result.op_type), (op.result.op_type==IS_CV ? op.result.u.var : -1)
	);

	if (detect_jump_pos(opa, position, &jmp1, &jmp2)) {
		if (jmp2 == -1) {
			php_printf(" j1->%-2d", jmp1);
		} else {
			php_printf(" j1->%-2d, j2->%-2d", jmp1, jmp2);
		}
	}

	php_printf("\n");
}
/* }}} */
