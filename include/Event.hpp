#pragma once
#include <list>
#include <functional>
#include <algorithm>
#include <memory>
template <class ...Params>
class Event: public std::enable_shared_from_this<Event<Params...>> {
public:
    using Handler = std::function<void(Params...)>;
    using Handler_id = std::list<Handler>::iterator;
private:
    std::list<Handler> handlers;
public:
    Event(): handlers() {}
    virtual ~Event() = default;
    class Connection {
        friend Event;
        std::weak_ptr<Event> event;
        Handler_id id;
    public:
	    Connection(const Connection& other) = delete;
	    Connection(Connection&& other) noexcept:
            event(other.event), id(other.id) {
            other.event.reset();
        }
	    Connection& operator=(const Connection& other) = delete;
	    Connection& operator=(Connection&& other) noexcept {
            this->event = other.event;
            this->id = other.id;
            other.event.reset();
            return *this;
        }
        Connection(Handler_id id, const std::shared_ptr<Event> event):
           event(event), id(id) {}
        Connection(): event(nullptr) {}
        ~Connection() {
            disconnect();
        }
        void disconnect() {
            if (auto ev = event.lock()) { ev->disconnect(id); }
        }
    };
    Connection connect(Handler handler) {
        auto id = handlers.insert(handlers.end(), std::move(handler));
        return Connection{id, this->shared_from_this()}; // apparently need to explicitly call 'this' with templated classes
    }
    void disconnect(Handler_id id) {
        handlers.erase(id);
    };
    void disconnect_all() {
        handlers.clear();
    }
    void fire_signal(Params ...args) {
        std::ranges::for_each(handlers, [&args...](Handler& handler) {
            handler(args...);
        });
    }
};
