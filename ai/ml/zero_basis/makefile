.PHONY: i pi

ARGS := $(wordlist 2, $(words $(MAKECMDGOALS)), $(MAKECMDGOALS))

i:
	python3 -m pip install $(ARGS)

pi:
	python -m pipenv install $(ARGS)
		

