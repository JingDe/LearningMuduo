#ifndef CHANNEL_H_
#define CHANNEL_H_

#include"TimerQueue.muduo/Timestamp.h"

#include<ctime>
#include<memory>
#include<string>

class EventLoop;


class Channel {
public:
	typedef std::function<void()> EventCallback;
	typedef std::function<void(Timestamp)> ReadEventCallback;

	Channel(EventLoop* loop, int fd);
	~Channel();

	void tie(const std::shared_ptr<void>& obj);
	void set_revents(int revt) { revents_ = revt; }
	void set_index(int index) { index_ = index;  }

	void handleEvent(Timestamp receiveTime);

	void update();
	void enableReading() { events_ |= kReadEvent; update(); }
	void enableWriting() { events_ |= kWriteEvent; update(); }
	void disableReading() { events_ |= ~kReadEvent; update(); }
	void disableWriting() { events_ |= ~kWriteEvent; update(); }
	void disableAll() { events_ = kNoneEvent; update(); }

	bool isWriting() const { return events_ & kWriteEvent; }
	bool isNoneEvent() const { return events_ == kNoneEvent;  }

	int fd() const { return fd_;  }
	int index() const { return index_;  }
	int events() const { return events_;  }
	int revents() const { return revents_;  }
	EventLoop* ownerLoop() { return loop_;  }

	std::string eventsToString() const;
	std::string reventsToString() const;

	void setReadCallback(const ReadEventCallback& cb) { readCallback_ = cb;  }
	void setWriteCallback(const EventCallback& cb) { writeCallback_ = cb; }
	void setCloseCallback(const EventCallback& cb) { closeCallback_ = cb; }
	void setErrorCallback(const EventCallback& cb) { errorCallback_ = cb; }

	void remove();

private:
	void handleEventWithGuard(Timestamp receiveTime);
	std::string eventsToString(int fd, int ev) const;

	EventLoop* loop_;

	const int fd_; // Channel管理的文件描述符
	int events_; // 关心的事件
	int revents_; // epoll或poll接受的事件

	int index_; // 在PollPoller类中表示该Channel在PollPoller类pollfd_中的下标
				// 在EPollPoller类中，取值kNew kAdded kDeleted，被用为标记channel是否在epoll的关注列表中

	
	bool tied_;
	std::weak_ptr<void> tie_;

	bool eventHandling_;

	bool addedToLoop_;

	ReadEventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback closeCallback_; // TcpConnection::handleClose()
	EventCallback errorCallback_; // TcpConnection::handleError()
	

	static const int kReadEvent; // 输入事件 POLLIN |  POLLPRI
	static const int kWriteEvent; // 输出事件 POLLOUT
	static const int kNoneEvent; // 0
};

/*
int poll(struct pollfd* fds, nfds_t nfds, int timeout);
struct pollfd{
	int fd;
	short events;
	short revents; // 由内核设置的事件，可以包含events或者POLLERR, POLLHUP, POLLNVAL
};
POLLERR:发生错误
POLLHUP:hang up
POLLNVAL:fd不是打开的描述符

*/

#endif
