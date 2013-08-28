watch('.*\.[ch]') { |f| system("make test NO_INTERACTION=1 TESTS='--show-all'") }
watch('tests/.*\.phpt') { |f| system("make test NO_INTERACTION=1 TESTS='--show-all'") }
