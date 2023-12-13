all: frontend middleend backend backend2

frontend:
	cd frontend && make && cd ..

middleend:
	cd middleend && make && cd ..

backend:
	cd backend && make && cd ..

backend2:
	cd backend2 && make && cd ..

.PHONY: frontend middleend backend backend2

run:
	cd frontend/ && ./frontend.out
	cd middleend/ && ./middleend.out
	cd backend/ && ./compiler.out
	cd backend2/ && ./cpu.out

clean:
	cd middleend && make clean && cd ..
	cd frontend && make clean && cd ..
	cd backend && make clean && cd ..
	cd backend2 && make clean && cd ..

