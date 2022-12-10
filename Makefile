all:
split:
	@bash "./scripts/split.bash" "./examples/example.inject.ino" ./
doc:
	@doxygen docs/doxygen.conf