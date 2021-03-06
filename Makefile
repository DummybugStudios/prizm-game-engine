include $(FXCGSDK)/toolchain/prizm_rules
OUTPUT 	:= $(CURDIR)/lib/libengine.a

BUILD	:= build
INCLUDES:= $(CURDIR)/include $(LIBFXCG_INC)
SOURCES := src

CFLAGS  := -c -Os -Wall $(MACHDEP) $(addprefix -I, $(INCLUDES)) -ffunction-sections -fdata-sections -g -Wno-unused-function -Wno-strict-aliasing

CFILES  := $(foreach dir, $(SOURCES), $(notdir $(wildcard $(dir)/*.c)))
OBJECTS := $(addprefix $(BUILD)/,$(CFILES:.c=.o))

DEPSDIR	:= $(CURDIR)/build
VPATH 	:= $(CURDIR)/$(SOURCES)

.PHONY: all clean libengine

all: $(OUTPUT)

$(OUTPUT): $(BUILD) $(OBJECTS)
	$(AR) rsv $@ $(OBJECTS)

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