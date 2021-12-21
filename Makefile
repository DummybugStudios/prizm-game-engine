include $(FXCGSDK)/toolchain/prizm_rules

OUTPUT 	:= $(CURDIR)/lib/libengine.a

BUILD	:= build
INCLUDES:= $(CURDIR)/include $(LIBFXCG_INC)

CFLAGS  := -Os -Wall $(MACHDEP) $(addprefix -I, $(INCLUDES)) -ffunction-sections -fdata-sections -g

CFILES  := $(foreach dir, src, $(notdir $(wildcard $(dir)/*.c)))
OBJECTS := $(addprefix $(BUILD)/,$(CFILES:.c=.o))

DEPSDIR	:= $(CURDIR)/build
VPATH 	:= $(CURDIR)/src

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(BUILD) $(OBJECTS)
	$(AR) rsv $@ $(OBJECTS)
	# $(addprefix $(BUILD)/, $(OBJECTS))

$(BUILD):
	@mkdir -p $@

# Changed the rule to make the .o files in the build folder
$(BUILD)/%.o: %.c
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c $< -o $@ 
	#$(BUILD)/$@ $(ERROR_FILTER)

DEPENDS := $(addprefix $(BUILD)/, $(OBJECTS:.o=.d))
-include $(DEPENDS)

clean:
	$(call rm, $(OUTPUT))
	$(call rmdir,$(BUILD))