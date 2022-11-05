check: check-unit check-functional

check-unit:
	build/test_hangman

check-functional:
	python3 test-bonus.py test hangman
	python3 test-bonus.py test hangman --no-global-config

clean:
	rm -rf test-*/*.out_produced
