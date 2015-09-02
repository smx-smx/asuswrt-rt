#
# Makefile for MIPS-specific library files..
#
obj-y := softfloat.o

# soft-float 
FPBIT_FUNCS = _pack_sf _unpack_sf _addsub_sf _mul_sf _div_sf \
    _fpcmp_parts_sf _compare_sf _eq_sf _ne_sf _gt_sf _ge_sf \
    _lt_sf _le_sf _unord_sf _si_to_sf _sf_to_si _negate_sf _make_sf \
    _sf_to_df _sf_to_tf _thenan_sf _sf_to_usi _usi_to_sf

DPBIT_FUNCS = _pack_df _unpack_df _addsub_df _mul_df _div_df \
    _fpcmp_parts_df _compare_df _eq_df _ne_df _gt_df _ge_df \
    _lt_df _le_df _unord_df _si_to_df _df_to_si _negate_df _make_df \
    _df_to_sf _df_to_tf _thenan_df _df_to_usi _usi_to_df

#LIB2FUNCS_1 = _muldi3 _negdi2 _lshrdi3 _ashldi3 _ashrdi3 _ffsdi2 _clz \
#    _cmpdi2 _ucmpdi2 _floatdidf _floatdisf _fixunsdfsi _fixunssfsi \
#    _fixunsdfdi _fixdfdi _fixunssfdi _fixsfdi _fixxfdi _fixunsxfdi
LIB2FUNCS_1 = _muldi3 _negdi2 _ffsdi2 _clz \
    _cmpdi2 _floatdidf _floatundidf _floatdisf _fixunsdfsi _fixunssfsi \
    _fixunsdfdi _fixdfdi _fixunssfdi _fixsfdi _fixxfdi _fixunsxfdi


LIB2FUNCS_2 = _floatdixf _fixunsxfsi _fixtfdi _fixunstfdi _floatditf \
    _trampoline _absvsi2 _absvdi2 _addvsi3 \
    _addvdi3 _subvsi3 _subvdi3 _mulvsi3 _mulvdi3 _negvsi2 _negvdi2
# These might cause a divide overflow trap and so are compiled with
# unwinder info.
LIB2_DIVMOD_FUNCS = _divdi3 _moddi3 _udivdi3 _umoddi3 _udiv_w_sdiv _udivmoddi4

DP_OBJS := $(patsubst %,%.o, $(DPBIT_FUNCS))
FP_OBJS := $(patsubst %,%.o, $(FPBIT_FUNCS))
GCC_OBJS := $(patsubst %,%.o, $(LIB2FUNCS_1)) $(patsubst %,%.o, $(LIB2FUNCS_2)) $(patsubst %,%.o, $(LIB2_DIVMOD_FUNCS))
softfloat-objs := $(DP_OBJS) $(FP_OBJS) $(GCC_OBJS) 
clean-files := dp-bits.c fp-bits.c
# end softfloat

# soft-float 
$(addprefix $(obj)/,$(patsubst %,%.o, $(DPBIT_FUNCS))): $(obj)/dp-bits.c
	$(CC) -c $(NOSTDINC_FLAGS) $(CPPFLAGS) $(CFLAGS) -DFINE_GRAINED_LIBRARIES -DL$(notdir $(patsubst %.o,%, $@)) $(obj)/dp-bits.c -o $@

$(addprefix $(obj)/,$(patsubst %,%.o, $(FPBIT_FUNCS))): $(obj)/fp-bits.c
	$(CC) -c $(NOSTDINC_FLAGS) $(CPPFLAGS) $(CFLAGS) -DFINE_GRAINED_LIBRARIES -DL$(notdir $(patsubst %.o,%, $@)) $(obj)/fp-bits.c -o $@

$(addprefix $(obj)/,$(patsubst %,%.o, $(LIB2FUNCS_1))): $(obj)/libgcc2.c
	$(CC) -c $(NOSTDINC_FLAGS) $(CPPFLAGS) $(CFLAGS) -DFINE_GRAINED_LIBRARIES -DL$(notdir $(patsubst %.o,%, $@)) $(obj)/libgcc2.c -o $@

$(addprefix $(obj)/,$(patsubst %,%.o, $(LIB2FUNCS_2))): $(obj)/libgcc2.c
	$(CC) -c $(NOSTDINC_FLAGS) $(CPPFLAGS) $(CFLAGS) -DFINE_GRAINED_LIBRARIES -DL$(notdir $(patsubst %.o,%, $@)) $(obj)/libgcc2.c -o $@
$(addprefix $(obj)/,$(patsubst %,%.o, $(LIB2_DIVMOD_FUNCS))): $(obj)/libgcc2.c
	$(CC) -c $(NOSTDINC_FLAGS) $(CPPFLAGS) $(CFLAGS) -DFINE_GRAINED_LIBRARIES -DL$(notdir $(patsubst %.o,%, $@)) $(obj)/libgcc2.c -o $@	
$(obj)/dp-bits.c: $(src)/fp-bit.c
	cp $(src)/fp-bit.c $(obj)/dp-bits.c

$(obj)/fp-bits.c: $(src)/fp-bit.c
	-echo '#define FLOAT' > $@
	cat $< >> $@

# end softfloat


