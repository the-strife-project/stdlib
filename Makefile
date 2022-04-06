export PROJNAME := stdlib
export RESULT := libstd.so
export STATICRESULT := libstd.a
export asm := yes
export lib := yes
export nostdlib := true

all: $(RESULT)

%: force
	@$(MAKE) -f ../helper/Makefile $@ --no-print-directory
force: ;
