.PHONY: clean

# Target to remove build, .cache and bin directories
clean:
	@echo "Removing build and .cache directories..."
	@rm -rf build .cache bin
	@echo "Clean complete."