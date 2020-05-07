#ifndef PRODCONS_HPP
#define PRODCONS_HPP

#include <mutex>
#include <queue>
#include <limits>

template<typename T>
class ProdCons {

	private :

		std::queue<T> queue;
		unsigned int TAILLE_MAX;
		std::mutex m;
		std::condition_variable c1;
		std::condition_variable c2;

	public :

		ProdCons(const unsigned int tMax = 100 ) : TAILLE_MAX(tMax) {}

		T get() {
			std::unique_lock<std::mutex> lck(m);

			while(queue.size() <= 0) {
				c1.wait(lck);
			}

			T temp = queue.front();
			queue.pop();
			c2.notify_all();
			return temp;
		}

		void put(const T el) {
			std::unique_lock<std::mutex> lck(m);

			while(queue.size() >= TAILLE_MAX) {
				c2.wait(lck);
			}

			queue.push(el);
			c1.notify_all();
		}
};

#endif // PRODCONS_HPP
