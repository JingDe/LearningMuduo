#include"Poller.h"
#include"EventLoop.h"
#include"Channel.h"

Poller::Poller(EventLoop* loop) :ownerLoop_(loop)
{

}

Poller::~Poller()
{}

void Poller::assertInLoopThread() const
{
	ownerLoop_->assertInLoopThread();
}

bool Poller::hasChannel(Channel* channel) const
{
	assertInLoopThread();
	ChannelMap::const_iterator it = channels_.find(channel->fd());
	return it != channels_.end() && it->second == channel;
}