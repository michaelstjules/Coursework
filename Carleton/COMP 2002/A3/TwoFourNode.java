import java.util.*;
import java.lang.*;

public class TwoFourNode<T> implements Comparable<TwoFourNode<T>> {
    private TwoFourNode<T> 				parent;
    private ArrayList<TwoFourNode<T>> 	children;
    private ArrayList<T> 				items;

    public TwoFourNode() {
    	children = new ArrayList<TwoFourNode<T>>(5); //room for one extra before splitting
    	items = new ArrayList<T>(4); //also, room for one extra before splitting
    	parent = null;
    }

	//intended to allow use of Collections.sort(u.getChildren()), but the latter was not used in the end
	//only to be used on sibling nodes, for which their sorted sets of items listed in order form a sorted set, if the nodes are distinct
	//should technically be using the tree's comparator
	//but since this isn't an inner class, it would have to be passed into the constructor, which is lame :)
	public int compareTo(TwoFourNode<T> u) {
		if(u==null)
			throw new NullPointerException();
			//throw new TwoFourNodeException("Parameter node is null.");
		if((u.getItems().get(0)==null) || (items.get(0)==null))
			throw new TwoFourNodeException("Empty node.");
		if(this.parent!=u.getParent())
			throw new TwoFourNodeException("These nodes are not in the same array.");
		if(this==u)
			return 0;
		if(((Comparable<T>)items.get(0)).compareTo(u.getItems().get(0))<0)
			return -1;
		if(((Comparable<T>)items.get(0)).compareTo(u.getItems().get(0))>0)
			return 1;
		throw new TwoFourNodeException("Something went wrong with these nodes.");
	}

	public boolean equals(TwoFourNode<T> u) {
		return this==u;
	}

    public ArrayList<T> getItems() {
    	return items;
    }

    public TwoFourNode<T> getParent() {
        return parent;
    }

    public void setParent(TwoFourNode<T> u) {
    	parent = u;
    }

    public ArrayList<TwoFourNode<T>> getChildren() {
    	return children;
    }

	//size (number of elements, not number of nodes) of the subtree rooted at this node
    public int size() {
		//BASE CASES
		if((parent==null) && (children.isEmpty()))
			return items.size(); //0 or 1
		if(children.isEmpty()) //if u is a leaf
			return 1;
		//RECURSIVE
		int n = 0; //0 for current node, since not external/leaf
    	for(TwoFourNode<T> u: children)
    		n += u.size();
    	return n;
    }

    //TO-DO: Create a string which includes the parent of the node, if any, plus, all of its children.
    //The format should be easy to follow and useful. Think about at as a helper method for debugging/testing
    public String  toString() {
    	String s = "{" + this.items;
    	if(parent==null)
    		s += ";ROOT";
    	else s += ";Parent =" + parent.getItems().toString();
    	s += ";Children:";
    	for(TwoFourNode<T> u: children)
    		s += u.getItems();
    	s+= "}";
    	return s;
    }
}