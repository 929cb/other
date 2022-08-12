src = $(wildcard ./*.c)
obj = $(patsubst ./%.c, ./%.o, $(src))
target = a.out

ALL:$(target)

$(target):$(obj)
	gcc $^ -o $@ -lmylib -L./ -g

$(obj):./%.o:./%.c
	gcc -c $< -o $@ -fPIC


path:
	export LD_LIBRARY_PATH=./

clean:
	-rm -rf $(obj) $(target)

.PHONY: clean ALL
