SHELL = cmd

TEMP = Temp
BIN = Bin
SOURCE = Source
TESTS = Tests

NAME = Collections
DICT_NAME = Dictionary
CARRAY_NAME = CArray
ARRAY_NAME = Array
TESTS_NAME = Tests

DLL := $(BIN)/lib$(NAME).dll
TESTS_EXE := $(BIN)/$(TESTS_NAME).exe

LOCAL_HEADERS := $(abspath $(wildcard */*.h Libraries/*/*/*.h))
LOCAL_INCLUDE := $(dir $(addprefix -I,$(LOCAL_HEADERS)))


All: $(DLL) $(TESTS_EXE)
	$(TESTS_EXE)

$(DLL): $(LOCAL_HEADERS) $(SOURCE)/$(DICT_NAME).c $(SOURCE)/$(ARRAY_NAME).c $(SOURCE)/$(CARRAY_NAME).c
	gcc -fPIC -c $(SOURCE)/$(DICT_NAME).c -o $(TEMP)/$(DICT_NAME).o
	gcc -fPIC -c $(SOURCE)/$(ARRAY_NAME).c -o $(TEMP)/$(ARRAY_NAME).o
	gcc -fPIC -c $(SOURCE)/$(CARRAY_NAME).c -o $(TEMP)/$(CARRAY_NAME).o
	gcc -s -shared $(TEMP)/$(ARRAY_NAME).o $(TEMP)/$(CARRAY_NAME).o $(TEMP)/$(DICT_NAME).o -o $(DLL)

$(TESTS_EXE): $(LOCAL_HEADERS) $(DLL) $(TESTS)/$(TESTS_NAME).c
	gcc -c $(LOCAL_INCLUDE) $(TESTS)/$(TESTS_NAME).c -o $(TEMP)/$(TESTS_NAME).o
	gcc -s -L$(BIN) -l$(NAME) $(TEMP)/$(TESTS_NAME).o -o $(TESTS_EXE)

Clean:
	DEL /Q $(BIN)\*.dll $(BIN)\*.exe
	DEL /Q $(TEMP)\*.o