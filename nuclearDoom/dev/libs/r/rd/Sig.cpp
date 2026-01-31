#include <stdafx.h>
#include "Sig.hpp"

rd::Sig::Sig() {
}

rd::SignalHandler* rd::Sig::add(std::function<void(void)> f) {
	SignalHandler* sig = new SignalHandler(f);
	signals.push_back(sig);
	return sig;
}
rd::SignalHandler* rd::Sig::add(const char * id,std::function<void(void)> f) {
	SignalHandler* sig = new SignalHandler(f);
	sig->name = id;
	signals.push_back(sig);
	return sig;
}

void rd::Sig::remove(rd::SignalHandler* handler) {
	if (!handler) return;

	auto idx = std::find(signals.begin(), signals.end(), handler);
	if (idx != signals.end()) {
		signals.erase(idx);
		delete handler;
		return;
	}
	
	idx = std::find(signalsOnce.begin(), signalsOnce.end(), handler);
	if (idx != signalsOnce.end()) {
		signalsOnce.erase(idx);
		delete handler;
		return;
	}
}



void rd::Sig::remove(const char* id){
	for (auto s : signals)
		if (s->name == id) {
			remove(s);
			return;
		}

	for (auto s : signalsOnce)
		if (s->name == id) {
			remove(s);
			return;
		}
}

rd::SignalHandler* rd::Sig::addOnce(std::function<void(void)> f) {
	SignalHandler* sig = new SignalHandler(f);
	signalsOnce.push_back(sig);
	return sig;
}

void rd::Sig::trigger() {
	isTriggering = true;
	triggerredCount++;
	for (auto s : signals) 
		s->function();
	if (signalsOnce.size() > 0) {
		for (auto s : signalsOnce) s->function();
		for (auto s : signalsOnce) delete s;
		signalsOnce.clear();
	}
	isTriggering = false;
}

void rd::Sig::operator()() {
	trigger();
}

void rd::Sig::clear(){
	if(signals.size())
	for (auto sig : signals)
		delete sig;

	if (signalsOnce.size())
	for (auto sigOnce : signalsOnce)
		delete sigOnce;

	signals.clear();
	signalsOnce.clear();
}

rd::Sig::~Sig() {
	for (auto sig : signals)
		delete sig;
	for (auto sigOnce : signalsOnce)
		delete sigOnce;

	signals.clear();
	signalsOnce.clear();
}

void rd::Sig::im() const {
	using namespace ImGui;
	Value("signals", signals.size());
	if (TreeNode(">##sig")) {
		for (auto& s : signals)
			Text(s->name);
		TreePop();
	}
	Value("signalOnces", signalsOnce.size());
	if (TreeNode(">##sig_once")) {
		for (auto& s : signalsOnce)
			Text(s->name);
		TreePop();
	}
}

void rd::Sig::im() {
	//we could have used a const cast but c++ does automatic const cast to const
	//the const cast is here to avoid to duplicate the function body, which cannot be done
	//unless we would have used proxy or template
	//sadly proxying or templating the call is very high on compiler pressure so there is no added value to it
	//unless you KNOW what you do don't try this at home ^^"
	const rd::Sig* cthis = this;
	cthis->im();//calls to the const version
}

rd::SignalHandler::SignalHandler(std::function<void(void)>& f) {
	function = f;
}

