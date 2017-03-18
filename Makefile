CONFIG=debug
CMAKELISTDIR=$(CURDIR)
OUT=build/$(CONFIG)

all: build

.PHONY: up
up:
	docker-compose up -d --build develop

.PHONY: down
down:
	docker-compose down -t develop

.PHONY: shell
shell: up
	docker-compose exec --user=`id -u`:`id -g` develop "bash"

.PHONY: cmake
cmake:
	mkdir -p $(OUT) 	\
		&& cd $(OUT) 	\
		&& cmake $(CMAKELISTDIR)

.PHONY: build
build: cmake
	cd $(OUT) \
		&& $(MAKE) CONFIG=$(CONFIG)

.PHONY: test
test: build
	cd $(OUT) \
		&& ctest --verbose

.PHONY: clean
clean:
	rm -rf $(OUT)
