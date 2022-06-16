/* 
 * @File tape.h
 * @Brief Tape class for AAD implementation
 * @Author Shane Keane
 * @Version 1.0  
 * @Date 06-06-2022
 *
 */

#include "Blocklist.h"
#include "Node.h"

constexpr size_t  BLOCKSIZE = 16384; /* Number of Nodes in a block */
constexpr size_t  DATASIZE  = 65536; /* Size of data in a block */

class Tape {
	/* Storage for the nodes */
	blocklist<Node, BLOCKSIZE> myNodes;
	/* Storage for data on the nodes (derivatives and child adjoint pointers) */
	blocklist<double , DATASIZE> myDers;
	blocklist<double*, DATASIZE> myArgPtrs;
	/* Padding to ensure no unnecessary overwriting */
	char myPad[64];
public: 
	template<size_t N>
	/* @Brief Build node in place and return pointer to it
	 * N is the number of children */
	Node* recordNode() {
		Node* node = myNodes.emplace_back(N);
		if constexpr(N) {
			node->pDerivatives = myDers.emplace_back_multi<N>();
			node->pAdjPtrs = myArgPtrs.emplace_back_multi<N>();
		}
		return node;
	}
	/* @Brief Function to reset adjoints */	
	void resetAdjoints() {
		for(Node& node : myNodes){
			node.mAdjoint = 0;
		}
	}
	/* Clear all data in blocklists */
	void clear() {
		myDers.clear();
		myArgPtrs.clear();
		myNodes.clear();
	}
	/* @Brief Rewind all blocklists together */
	void rewind() {
		myDers.rewind();
		myArgPtrs.rewind();
		myNodes.rewind();
	}
	/* @Brief Set mark for all blocklists */
	void mark() {
		myDers.setmark();
		myArgPtrs.setmark();
		myNodes.setmark();
	}
	/* @Brief Rewind to mark on all blocklists */
	void RewindToMark() {
		myDers.rewind_to_mark();
		myArgPtrs.rewind_to_mark();
		myNodes.rewind_to_mark();
	}
	/* @Brief iterators */
	using iterator = blocklist<Node, BLOCKSIZE>::iterator;
	auto begin() {
		return myNodes.begin();
	}
	auto end() {
		return myNodes.end();
	}
	auto markIt() {
		return myNodes.mark();
	}
	auto find(Node* node) {
		return myNodes.find(node);
	}
};
