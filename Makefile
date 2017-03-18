CONFIG=debug
OUT=cmake-build-$(CONFIG)

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
cmake: up
	mkdir -p $(OUT) 	\
		&& cd $(OUT) 	\
		&& cmake ../

.PHONY: build
build: cmake
	cd $(OUT) \
		&& $(MAKE) CONFIG=$(CONFIG)

.PHONY: clean
clean:
	rm -rf $(OUT)
