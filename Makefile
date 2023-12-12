all: frontend middleend

frontend:
	cd frontend && make && cd ..

middleend:
	cd middleend && make && cd ..

.PHONY: frontend middleend

run:
	cd frontend/ && ./frontend.out
	cd middleend/ && ./middleend.out

clean:
	cd middleend && make clean && cd ..
	cd frontend && make clean && cd ..

