TEST_DIRS = rcc

.PHONY: all test clean $(TETS_DIRS)
all: test

test:
	@set -e; \
	for dir in $(TEST_DIRS); do \
		echo ""; \
		echo "============================================="; \
		echo "Running test: $$dir"; \
		echo "=============================================="; \
		$(MAKE) -C $$dir test; \
	done

clean:
	@for dir in $(TEST_DIRS); do \
		echo ""; \
		echo "Cleaning $$dir..."; \
		$(MAKE) -C $$dir clean; \
	done