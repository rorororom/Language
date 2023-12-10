all: frontend

frontend:
	cd frontend && make && cd ..

.PHONY: frontend

run:
	cd frontend/ && ./differentiation.out

clean:
	cd frontend && make clean && cd ..

