# This library requires the export of the variables TEMP and BIN

SOURCE = Source

NAME = Collections
DICT_NAME = Dictionary
CARRAY_NAME = CArray
ARRAY_NAME = Array

DLL := $(BIN)/lib$(NAME).dll

LOCAL_HEADERS := $(abspath $(wildcard */*.h Libraries/*/*/*.h))
LOCAL_INCLUDE := $(dir $(addprefix -I,$(LOCAL_HEADERS)))

All: $(DLL)

$(DLL): $(LOCAL_HEADERS) $(SOURCE)/$(DICT_NAME).c $(SOURCE)/$(ARRAY_NAME).c $(SOURCE)/$(CARRAY_NAME).c
	gcc -fPIC -c $(SOURCE)/$(DICT_NAME).c -o $(TEMP)/$(DICT_NAME).o
	gcc -fPIC -c $(SOURCE)/$(ARRAY_NAME).c -o $(TEMP)/$(ARRAY_NAME).o
	gcc -fPIC -c $(SOURCE)/$(CARRAY_NAME).c -o $(TEMP)/$(CARRAY_NAME).o
	gcc -s -shared $(TEMP)/$(ARRAY_NAME).o $(TEMP)/$(CARRAY_NAME).o $(TEMP)/$(DICT_NAME).o -o $(DLL)