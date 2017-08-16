#include "VNode.hpp"
#ifndef ASMDOM_JS_SIDE
	#include <emscripten/val.h>
	#include <emscripten/bind.h>
#endif
#include <cstdint>
#include <string>

namespace asmdom {

	void addNS(VNode* const vnode) {
		vnode->data.attrs["ns"] = "http://www.w3.org/2000/svg";
		if (vnode->sel.compare("foreignObject") != 0 && !vnode->children.empty()) {
			for(std::vector<VNode*>::size_type i = 0; i != vnode->children.size(); ++i) {
				addNS(vnode->children[i]);
			}
		}
	}

	void VNode::adjustVNode() {
		if (data.attrs.count(std::string("key")) != 0) {
			key = data.attrs["key"];
		}

		if (sel[0] == 's' && sel[1] == 'v' && sel[2] == 'g') {
			addNS(this);
		}

		children.erase(std::remove(children.begin(), children.end(), (VNode*)NULL), children.end());
	};

	void deleteVNode(VNode* vnode) {
    std::vector<VNode*>::size_type i = vnode->children.size();
    while (i--) deleteVNode(vnode->children[i]);
    delete vnode;
  };

	#ifndef ASMDOM_JS_SIDE

		emscripten::val functionCallback(const std::uintptr_t& vnode, const std::string& callback, emscripten::val event) {
			return emscripten::val(reinterpret_cast<VNode*>(vnode)->data.callbacks[callback](event));
		};

		EMSCRIPTEN_BINDINGS(function_callback) {
			emscripten::function("functionCallback", &functionCallback, emscripten::allow_raw_pointers());
		};

	#endif

}
