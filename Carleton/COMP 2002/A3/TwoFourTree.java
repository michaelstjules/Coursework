//MICHAEL ST. JULES, COMP2002/2402 SUMMER 2012, ASSIGNMENT 3

//IMPLEMENTED AS A B+ TREE, AS DESCRIBED IN NOTES (ALL KEYS ON LEAVES)
//The child with the index corresponding to its parents item/keys contains items/keys less than or equal to that item/key
//The last child contains items/keys strictly greater than all of the parents items/keys

//THEOREM 1: THE MOST RECENT COMMON ANCESTOR OF A LEAF AND ITS NEXT IS THE ONLY OTHER NODE CONTAINING THE LEAF'S ITEM/KEY
	//THIS MEANS ITEMS CAN BE FOUND FASTER (SOMETIMES)
//SKETCH OF PROOF:
//TO PROVE EXISTENCE:
	//If the leaf is not the last/rightmost of its siblings, then its parent contains its item/key, x.
	//Otherwise, we check the next ancestor: if its parent contains x, then we move to the ancestor's right sibling, because its branch contains the smallest value strictly greater than x (in this implementation).
	//If not found at this ancestor, continue up to the next ancestor, and so on.
	//If we hit the root and x is still not found, then there is no next, so x is the greatest.
	//Otherwise continue to the leftmost descendant of the right sibling of the ancestor we found.
//TO PROVE UNIQUENESS:
	//Suppose, by way of contradiction, there were another internal node containing x, besides the ancestor we found.
	//If this node's depth is higher/height is lower, then the tree cannot represent a sortedset, because x would exist in more than one leaf.
	//Otherwise, this node's depth is lower/height is higher, and so it cannot be followed to the leaf's next, since the ancestor we found should lead to it; the path would pass it to the right

//THEOREM 2: THE ITEMS/KEYS IN THE INTERNAL NODES ARE THE GREATEST ITEMS/KEYS IN THE CORRESPONDING BRANCHES
//SKETCH OF PROOF:
	//By THEOREM 2, a leaf that is not the last/rightmost of its siblings has its item/key contained in its parent, corresponding to it alone.
	//By THEOREM 2, a leaf that is the last/rightmost of its siblings has its item/key also stored in the ancestor that leads to the next leaf.
	//Traversing to the corresponding child from that ancestor, and then down through the rightmost children leads back to the leaf.



import java.util.*;


public class TwoFourTree<T> implements SSet<T> {

	private TwoFourNode<T> root;
	private int n; // = 0;
	private Comparator<? super T> c;
	private T greatest, least; //makes for a lot more cases, but they're easy to implement and improve speed
	//could have used the nodes with greatest and least items, instead, though, to access them more quickly

	public TwoFourTree() {
		root = new TwoFourNode<T>();
		n = 0;
		this.c = new DefaultComparator<T>();
		greatest = null;
		least = null;
	}

	public TwoFourTree(Comparator<T> c) {
		root = new TwoFourNode<T>();
		n = 0;
		this.c = c;
		greatest = null;
		least = null;
	}

	//from ODS "DefaultComparator.java"
	private class DefaultComparator<T> implements Comparator<T> {
		public int compare(T a, T b) {
			return ((Comparable<T>)a).compareTo(b);
		}
		public boolean equals(Object o) {
			//if(!(o instanceof DefaultComparator))
			//	return false;
			return this==o;
		}
	}

	public Iterator<T> iterator() {
		return iterator(least);
	}

	public Comparator<? super T> comparator() { //had "? super Object" instead of "T"
		return c;
	}

	public int size() {
		return n;
	}

	public T getGreatest() {
		return greatest;
	}

	public T getLeast() {
		return least;
	}


	private TwoFourNode<T> findParentNode(T x) {
		if(root.getChildren().isEmpty()) //i.e. if n=0 or n=1
			throw new TwoFourNodeException("Cannot find parent node since root has no children.");
		TwoFourNode<T> u = root;
		while(!u.getChildren().get(0).getChildren().isEmpty()){ //u.getChildren() will be leaves after finishing the loop
			if(c.compare(x, u.getItems().get(u.getItems().size()-1))>0) //if x is greater than all of u's items
				u = u.getChildren().get(u.getChildren().size()-1); //go to last child
			else{
				//find the first item that's greater than or equal to x, and go to corresponding child
				//find the first item that x is less than or equal to, and go to corresponding child
				for(int i=0; i<u.getItems().size(); i++){
					if(c.compare(x, u.getItems().get(i))<=0){
						u = u.getChildren().get(i);
						break;
					}
				}
			}
		}
		return u;
	}


	//use loop and count steps up, and take as many steps back down
	private TwoFourNode<T> findNextNode(TwoFourNode<T> v) {
		if(v==null)
			return null;
		if(v==root)
			return null;
		TwoFourNode<T> u;
		int i;
		//go up until you find a node with a right sibling, counting the steps up
		//ArrayIndexOutOfBoundsException: -1, after merging
		for(u=v, i=0; u==u.getParent().getChildren().get(u.getParent().getChildren().size()-1); u=u.getParent(), i++){
			if(u.getParent()==root){ //avoid NullPointerException
				if(u==root.getChildren().get(root.getChildren().size()-1)) //if u is the root's rightmost child
					return null;
				break;
			}
		}
		//here, u is the first node along the path to the root that isn't the rightmost of its siblings
		int j = u.getParent().getChildren().indexOf(u);
		//move to its next sibling
		u = u.getParent().getChildren().get(j+1);
		//then go down to the left until you reach the same level as v
		while(i>0){
			u = u.getChildren().get(0);
			i--;
		}
		return u;
	}


	private TwoFourNode<T> findPreviousNode(TwoFourNode<T> v) {
		if(v==null)
			return null;
		if(v==root)
			return null;
		TwoFourNode<T> u;
		int i;
		//go up until you find a node with a left sibling, counting the steps up
		for(u=v, i=0; u==u.getParent().getChildren().get(0); u=u.getParent(), i++){
			if(u.getParent()==root){ //avoid NullPointerException
				if(u==root.getChildren().get(0)) //if u is the root's leftmost child
					return null;
				break;
			}
		}
		//here, u is the first node along the path to the root that isn't the leftmost of its siblings
		int j = u.getParent().getChildren().indexOf(u);
		//move to its previous sibling
		u = u.getParent().getChildren().get(j-1);
		//then go down to the right until you reach the same level as v
		while(i>0){
			u = u.getChildren().get(u.getChildren().size()-1);
			i--;
		}
		return u;
	}


	//Smallest greater than or equal to x (ceiling) or null if no such element exists
	//y>=x, so start at left of x
	//x would be in findParent(x), so y>=x would be in findParent(x) or the next node, never previous (since y<x in previous)
	public T find(T x) {
		if(x==null)
			return null;
		if(c.compare(x, least)<=0)
			return least;
		if(c.compare(x, greatest)>0)
			return null;
		if(c.compare(x, greatest)==0)
			return greatest;
		if(root.getChildren().isEmpty()){
			if(root.getItems().isEmpty())
				return null;
			//otherwise it's a leaf, with a single item
			if(c.compare(x, root.getItems().get(0))<=0)
				return root.getItems().get(0);
			return null;
		}
		//TwoFourNode<T> u = findParentNode(x);
		TwoFourNode<T> u = root;
		while(!u.getChildren().get(0).getChildren().isEmpty()){ //u.getChildren() will be leaves after finishing the loop
			if(c.compare(x, u.getItems().get(u.getItems().size()-1))>0) //if x is greater than all of u's items
				u = u.getChildren().get(u.getChildren().size()-1); //go to last child
			else{
				//find x, or the first item that's greater than it, and go to corresponding child
				for(int i=0; i<u.getItems().size(); i++){
					//if(c.compare(x, u.getItems().get(i))<=0){
					if(c.compare(x, u.getItems().get(i))==0){
						return u.getItems().get(i);
					}
					if(c.compare(x, u.getItems().get(i))<0){
						u = u.getChildren().get(i);
						break;
					}
				}
			}
		}
		for(TwoFourNode<T> w: u.getChildren()){
			if(c.compare(x, w.getItems().get(0))<=0)
				return w.getItems().get(0);
		}
		u = findNextNode(u);
		//return the item in the first child of the next node, or null
		return u==null ? null : u.getChildren().get(0).getItems().get(0);
	}


	//Smallest greater than or equal to x (ceiling) or least if x is null
	public T findGE(T x) {
		if(x==null)
			return least;
		return find(x);
	}


	//Largest strictly less than x or greatest (or smallest? discrepancy in notes) if x is null
	//y<x, so start at right of x
	//x would be in findParent(x), so y<x would be in findParent(x) or the previous node, never next (since y>x in next node)
	public T findLT(T x) {
		if(x==null)
			return greatest;
		if(c.compare(x, greatest)>0) //if x is greater than the greatest
			return greatest;
		if(c.compare(x, least)<=0) //if x is less than or equal to the least
			return null;
		TwoFourNode<T> parent = findParentNode(x);
		T y;
		//check from right to left until you find a y<x
		for(int i=parent.getChildren().size()-1; i>0; i--){
			if(c.compare(x, y=parent.getChildren().get(i).getItems().get(0))>0)
				return y;
		}
		parent = findPreviousNode(parent);
		//return the item in the last child of the previous node, or null
		return parent==null ? null : parent.getChildren().get(parent.getChildren().size()-1).getItems().get(0);
	}


	//split to right, moving 2 children, one key to new node, and one key up to parent
	//recursive as necessary, so only needs to be called on the parent of the new leaf
	//items are dealt with as in B-trees
	private void split(TwoFourNode<T> v) {
		//BASE CASES
		if(v.getChildren().size()<5)
			return;
		if(v==root){ //make a new root
			TwoFourNode<T> newRoot = new TwoFourNode<T>();
			v.setParent(newRoot);
			newRoot.getChildren().add(v);
			root = newRoot;
			//proceed into general case
		}
		//GENERAL CASE
		TwoFourNode<T> u = new TwoFourNode<T>();
		int i = v.getParent().getChildren().indexOf(v);
		u.setParent(v.getParent());
		//move the 4th and 5th children of v to u
		TwoFourNode<T> v4, v5;
		v4 = v.getChildren().remove(v.getChildren().size()-2);
		v5 = v.getChildren().remove(v.getChildren().size()-1);
		u.getChildren().add(v4);
		u.getChildren().add(v5);
		v4.setParent(u);
		v5.setParent(u);
		u.getItems().add(v.getItems().remove(v.getItems().size()-1)); //move 4th key to u
		v.getParent().getChildren().add(i+1, u); //add u as the sibling to the right of v (even if v is last)
		v.getParent().getItems().add(i, v.getItems().remove(v.getItems().size()-1)); //move 3rd key to parent
		//if the parent now has 5 children, split it, too
		if(v.getParent().getChildren().size()==5)
			split(v.getParent());
	}


	//try to borrow before trying to merge, as in notes
	//recursive as necessary
	//items are dealt with as in B-trees
	private void borrow(TwoFourNode<T> v) {
		//BASE CASES
		if(v.getChildren().size()!=1) //also eliminates possibility of borrowing for leaves
			return;
		if(v==root){
			root = v.getChildren().get(0);
			root.setParent(null);
			//no need to deal with items
			return;
		}
		//GENERAL CASE
		TwoFourNode<T> parent = v.getParent();
		int i = parent.getChildren().indexOf(v); //u's index in its parent's children
		TwoFourNode<T> u;
		boolean uIsLeft; //true u is the left sibling, false if u is the right
		//find sibling with most children, and right in case of equality
		if((i>0) && (i<parent.getChildren().size()-1)){
			if(parent.getChildren().get(i+1).getChildren().size()>parent.getChildren().get(i-1).getChildren().size()){
				u = parent.getChildren().get(i+1);
				uIsLeft = false;
			}
			else{
				 u = parent.getChildren().get(i-1);
				 uIsLeft = true;
			}
		}
		else if(i>0){
			u = parent.getChildren().get(i-1);
			uIsLeft = true;
		}
		else{
			u = parent.getChildren().get(i+1);
			uIsLeft = false;
		}
		if(u.getChildren().size()<3){ //if neither has at least 3 children
			merge(v);
			return;
		}
		TwoFourNode<T> uchild;
		if(uIsLeft){
			uchild = u.getChildren().remove(u.getChildren().size()-1);
			v.getChildren().add(0, uchild); //move u's last child to v
			uchild.setParent(v);
			//move parent's item corresponding to u into v, and u's last item into parent's empty spot
			//u's last item goes up, replacing the one that used to correspond to u, and the item it replaces goes down into v
			v.getItems().add(parent.getItems().set(i-1, u.getItems().remove(u.getItems().size()-1)));
		}
		else{ //u is on the right
			uchild = u.getChildren().remove(0);
			v.getChildren().add(uchild); //move u's first child to v
			uchild.setParent(v);
			//move parent's item corresponding to v into v, and u's first item into parent's empty spot
			//u's first item goes up, replacing the one that used to correspond to v, and the item it replaces goes down into v
			v.getItems().add(parent.getItems().set(i, u.getItems().remove(0)));
		}
		//if the parent now has 1 child, borrow for (or merge) it
		if(parent.getChildren().size()==1)
			borrow(parent);
	}


	//recursive as necessary
	//items are dealt with as in B-trees
	private void merge(TwoFourNode<T> v) {
		//BASE CASES
		if(v.getChildren().size()!=1) //also eliminates possibility of merging leaves
			return;
		if(v==root){
			root = v.getChildren().get(0);
			root.setParent(null);
			//no need to deal with items
			return;
		}
		//GENERAL CASE
		//merge with the right, if both closest siblings have 2 children
		TwoFourNode<T> parent = v.getParent();
		int i = parent.getChildren().indexOf(v); //u's index in its parent's children
		TwoFourNode<T> u = null;
		boolean uIsLeft = false; //true u is the left sibling, false if u is the right
		//take a sibling with 2 children, with preference for right
		if((i>0) && (i<parent.getChildren().size()-1)){
			if(parent.getChildren().get(i+1).getChildren().size()==2){
				u = parent.getChildren().get(i+1);
			}
			else if(parent.getChildren().get(i-1).getChildren().size()==2){
				 u = parent.getChildren().get(i-1);
				 uIsLeft = true;
			}
		}
		else if((i>0) && (parent.getChildren().get(i-1).getChildren().size()==2)){
			u = parent.getChildren().get(i-1);
			uIsLeft = true;
		}
		else if((i<parent.getChildren().size()-1) && (parent.getChildren().get(i+1).getChildren().size()==2)){ //unnecessary check for index?
			u = parent.getChildren().get(i+1);
		}
		if(u==null){ //if neither has at exactly 2 children
			borrow(v);
			return;
		}
		//move v's child into u and remove v
		//the key that separated the two nodes at the parent is moved down into u
		if(uIsLeft){
			TwoFourNode<T> vchild = v.getChildren().remove(0);
			u.getChildren().add(vchild);
			vchild.setParent(u);
			//if u is on the left of v, then we want the item at i-1
			u.getItems().add(parent.getItems().remove(i-1));
		}
		else{
			TwoFourNode<T> vchild = v.getChildren().remove(0);
			u.getChildren().add(0, vchild);
			vchild.setParent(u);
			//if u is on the right of v, then we want the item at i
			u.getItems().add(0, parent.getItems().remove(i));
		}
		parent.getChildren().remove(i);
		//if the parent now has 1 child, borrow for (or merge) it
		if(parent.getChildren().size()==1)
			borrow(parent);
	}


	public boolean add(T x) {
		if(n==0){
			root.getItems().add(x);
			n++;
			greatest = x;
			least = x;
			return true;
			//the root is now a leaf with an item
		}
		if(n==1){ //if the root was just a leaf with an item; the only item==greatest==least
			if(c.compare(x, greatest)==0)
				return false;
			//make a new root and add previous root and new x node to it
			//greatest will be greatest of the two items and item in new root will be other item, the least
			TwoFourNode<T> newRoot = new TwoFourNode<T>();
			TwoFourNode<T> v = new TwoFourNode<T>();
			v.getItems().add(x);
			root.setParent(newRoot);
			v.setParent(newRoot);
			if(c.compare(x, greatest)>0){ //if x is greater than the current item
				newRoot.getItems().add(greatest);
				greatest = x;
				newRoot.getChildren().add(root);
				newRoot.getChildren().add(v);
			}
			else{
				newRoot.getItems().add(x);
				least = x;
				newRoot.getChildren().add(v);
				newRoot.getChildren().add(root);
			}
			root = newRoot;
			n++;
			return true;
		}
		if((c.compare(x, greatest)==0) || (c.compare(x, least)==0))
			return false;
		//copypasta find(x) code with some modification, so that if x is found earlier, we can simply stop and return false
		TwoFourNode<T> parent = root;
		while(!parent.getChildren().get(0).getChildren().isEmpty()){ //parent.getChildren() will be leaves after finishing the loop
			if(c.compare(x, parent.getItems().get(parent.getItems().size()-1))>0){ //if x is greater than all of parent's items
				parent = parent.getChildren().get(parent.getChildren().size()-1); //go to last child

			}
			else{
				//find x, or the first item that's greater than it, and go to corresponding child
				for(int i=0; i<parent.getItems().size(); i++){
					if(c.compare(x, parent.getItems().get(i))==0){
						return false;
					}
					if(c.compare(x, parent.getItems().get(i))<0){
						parent = parent.getChildren().get(i);
						break;
					}
				}
			}
		}
		//only need to check the last child, otherwise we would have found x earlier
		if(c.compare(x, parent.getChildren().get(parent.getChildren().size()-1).getItems().get(0))==0)
			return false;
		int i;
		for(i=0; (i<parent.getChildren().size()) && (c.compare(x, parent.getChildren().get(i).getItems().get(0))!=0); i++);
		if(i<parent.getChildren().size()){
			if(c.compare(x, parent.getChildren().get(i).getItems().get(0))==0) //if x was found
				return false;
		}
		TwoFourNode<T> u = new TwoFourNode<T>();
		u.getItems().add(x);
		u.setParent(parent);
		for(i=0; i<u.getParent().getChildren().size(); i++){
			if(c.compare(x, u.getParent().getChildren().get(i).getItems().get(0))<0)
				break;
		}
		parent.getChildren().add(i, u);
		//Collections.sort(parent.getChildren());
		//i = parent.getChildren().indexOf(u);
		//if u is not the last child; add its item to the parent in the corresponding index
		if(i<parent.getChildren().size()-1)
			parent.getItems().add(i, x);
		else{
			//add the second/previous last's item to the parent, and replace any higher occurence of this item with x
			T y = parent.getChildren().get(parent.getChildren().size()-2).getItems().get(0);
			parent.getItems().add(y);
			if(y==greatest){ //there is no higher occurrence of y
				greatest = x;
			}
			else{
				//pass the new item up to the ancestor that contained the second/previous leaf's item
				//i.e. just go up until y is found and replace it with x
				TwoFourNode<T> ancestor;
				for(ancestor=parent; !ancestor.getItems().contains(y); ancestor=ancestor.getParent());
				ancestor.getItems().set(ancestor.getItems().indexOf(y), x);
			}
		}
		if(c.compare(x, least)<0)
			least = x;
		n++;
		if(parent.getChildren().size()==5){
			split(parent); //recursive
		}
		return true;
	}


	public boolean remove(T x) {
		if(n==0)
			return false;
		if(n==1){
			if(c.compare(x, root.getItems().get(0))==0){
				root.getItems().remove(0);
				n--;
				greatest = null;
				least = null;
				return true;
			}
			return false;
		}
		if(n==2){
			boolean found = false;
			if(c.compare(x, root.getChildren().get(0).getItems().get(0))==0){
				root = root.getChildren().get(0);
				found = true;
			}
			else if(c.compare(x, root.getChildren().get(1).getItems().get(0))==0){
				root = root.getChildren().get(1);
				found = true;
			}
			if(found){
				root.setParent(null);
				greatest = root.getItems().get(0);
				least = greatest;
				n--;
				return true;
			}
			return false;
		}
		if((c.compare(x, least)<0) && (c.compare(x, greatest)>0))
			return false;
		TwoFourNode<T> parent = findParentNode(x);
		int i;
		for(i=0; (i<parent.getChildren().size()) && (c.compare(x, parent.getChildren().get(i).getItems().get(0))!=0); i++);
		//either x is at i or i=size()
		if(i<parent.getChildren().size()){
			if(c.compare(x, parent.getChildren().get(i).getItems().get(0))!=0)
				return false;
		}
		else return false;
		TwoFourNode<T> v = parent.getChildren().get(i);
		if(i<=parent.getItems().size()-1)
			parent.getItems().remove(i);
		//otherwise keep looking
		else{
			if(c.compare(x, greatest)==0){
				parent.getItems().remove(parent.getItems().size()-1); //remove the parent's last item
				greatest = parent.getChildren().get(parent.getChildren().size()-2).getItems().get(0); //greatest is now the second to last child's item
			}
			else{
				TwoFourNode<T> u;
				for(u=v.getParent(); !u.getItems().contains(x); u=u.getParent()); //keep moving up until u contains x
				//u is now the most recent common ancestor of v and its next, and it contains x
				//put v's parent's last key in the place of u's key that leads to v (i.e. x)
				u.getItems().set(u.getItems().indexOf(x), parent.getItems().remove(parent.getItems().size()-1));
			}
		}
		if(c.compare(x, least)==0)
			least = parent.getChildren().get(1).getItems().get(0);
		parent.getChildren().remove(i);
		n--;
		if(parent.getChildren().size()==1){
			borrow(parent); //recursive
		}
		return true;
	}


	public void clear() {
		root = new TwoFourNode<T>();
		n = 0;
		greatest = null;
		least = null;
	}


	public Iterator<T> iterator(T x) {
		class TFTIterator implements Iterator<T> {

			private TwoFourNode<T> v, prev;

			public TFTIterator(T x) {
				//starting at the first element that is greater than or equal to x.
				prev = null;
				if(n==0){
					v = null;
					return;
				}
				if(n==1){
					//if the root is a leaf, with a single item
					if(c.compare(x, root.getItems().get(0))<=0){
						v = root;
						return;
					}
					v = null;
					return;
				}
				TwoFourNode<T> parent = findParentNode(x);
				for(TwoFourNode<T> u: parent.getChildren()){
					if(c.compare(x, u.getItems().get(0))<=0){
						v = u;
						return;
					}
				}
				//otherwise go to node next to parent and check its first child
				parent = findNextNode(parent);
				if(parent!=null)
					v = parent.getChildren().get(0);
				else v = null;
			}

			public void remove() {
				if(prev==root){
					clear();
					prev = null;
					v = null;
					return;
				}
				TwoFourNode<T> newPrev = findPreviousNode(prev);
				if(c.compare(least, prev.getItems().get(0))==0){
					least = prev.getParent().getChildren().get(1).getItems().get(0);
				}
				if(c.compare(greatest, prev.getItems().get(0))==0){
					greatest = prev.getParent().getChildren().get(prev.getParent().getChildren().size()-2).getItems().get(0);
					prev.getParent().getItems().remove(prev.getParent().getItems().size()-1); //remove the parent's last item
				}
				else{
					int i = prev.getParent().getChildren().indexOf(prev);
					if(i<=prev.getParent().getItems().size()-1)
						prev.getParent().getItems().remove(i);
					else{
						//remove the key that would lead you here
						//and move parent's last key into its place
						TwoFourNode<T> u, w;
						//find the most recent common ancestor of prev and v (its next), but stop when u and w are siblings
						for(u=prev, w=v; u.getParent()!=w.getParent(); u=u.getParent(), w=w.getParent());
						i = u.getParent().getChildren().indexOf(u);
						u = u.getParent();
						//u is now the most recent common ancestor of prev and v
						//put prev's parent's last key in the place of u's key that leads to prev
						u.getItems().set(i, prev.getParent().getItems().remove(prev.getParent().getItems().size()-1));
					}
				}
				prev.getParent().getChildren().remove(prev);
				n--;
				if(prev.getParent().getChildren().size()==1)
					borrow(prev.getParent());
				prev = newPrev;
			}

			public T next() {
				prev = v;
				v = findNextNode(v);
				return prev.getItems().get(0); //this will be first item on first call
			}

			public T previous() {
				T x = v.getItems().get(0);
				v = prev;
				prev = findPreviousNode(prev);
				return v.getItems().get(0);
			}

			public boolean hasNext() {
				return v!=null;
				//return prev.getItems().get(0)!=greatest;
			}

			public boolean hasPrevious() {
				return prev.getItems().get(0)!=least;
			}
		}
		return new TFTIterator(x);
	}

	public boolean unionWith(SSet<T> sset) {
		//add all elements in sset to this set
		for(T x: sset)
			add(x);
		return true;
	}

	public boolean intersectWith(SSet<T> sset) {
		//remove all elements from this set that are not in sset
		Iterator<T> it = iterator();
		while(it.hasNext()){
			if(!(sset.belongsTo(it.next())))
				it.remove();
		}
		return true;
	}

	public boolean differenceWith(SSet<T> sset) {
		//remove all elements from this set that are in sset
		for(T x: sset)
			remove(x);
		return true;
	}

	public boolean subsetOf(SSet<T> sset) {
		//for each element in this set, check if it's in sset; return false on first not found in sset
		for(T x: this){
			if(!(sset.belongsTo(x)))
				return false;
		}
		return true;
	}

	public boolean belongsTo(T x) {
		if(x==null)
			return false;
		T y = find(x);
		if(y==null)
			return false;
		return (c.compare(x, y)==0) ? true : false;
	}

	public String recursiveToString(TwoFourNode<T> u) {
		//calls toString() on every node (but children), starting from u
		//preorder
		String s = u.toString(); //add this node's string
		//BASE CASES
		if(n==0)
			return null;
		if(u.getChildren().isEmpty())
			return "{Leaf:" + u.getItems().get(0) + "}";
		//RECURSIVE CASE
		for(TwoFourNode<T> v : u.getChildren()) //then add for each of its children
			s += ", " + recursiveToString(v);
		return s;
	}

	public String toString2() {
		return "least=" + least + "; greatest=" + greatest + "; " + recursiveToString(root);
	}

	//from ODS, although probably standard even in JCF
	//calls toString() on leaf items, only
	public String toString() {
		String s = "[";
		Iterator<T> it = iterator();
		while (it.hasNext()) {
			s += it.next().toString() + (it.hasNext() ? "," : "");
		}
		s += "]";
		return s;
	}

	private void update(TwoFourNode<T> v) {
		if(v.getChildren().isEmpty())
			return;
		TwoFourNode<T> u;
		v.getItems().clear();
		for(int i=0; i<v.getChildren().size()-1; i++){ //for each child but the last
			u = v.getChildren().get(i);
			//find the greatest (right-most) descendant leaf of this child
			while(!u.getChildren().isEmpty())
				u = u.getChildren().get(u.getChildren().size()-1);
			v.getItems().add(u.getItems().get(0)); //set it to this leaf's item
		}
	}

	private boolean verifyNode(TwoFourNode<T> v) { //a return value of true is not conclusive (because only ancestors and descendants are checked), but a return value of false is
		if(n==0)
			return ((v==root) && (v.getItems().isEmpty()) && (v.getChildren().isEmpty()));
		if(n==1)
			return ((v==root) && (v.getItems().size()==1) && (v.getChildren().isEmpty()));
		TwoFourNode<T> u;
		if(v.getChildren().isEmpty()){ //if it's a leaf, check that its item occurs in the correct ancestor
			if(v.getItems().size()!=1){
				System.out.println("Too few items in leaf: " + v);
				return false;
			}
			T x = v.getItems().get(0);
			if(x==greatest){ //if it contains the greatest item, then there should be no other occurences
				for(u=v.getParent(); u!=root; u=u.getParent()){
					if(u.getItems().contains(x)){
						System.out.println("Found greatest elsewhere: " + x + " " + u);
						return false;
					}
				}
				return true;
			}
			//check its parent
			if((v.getParent().getChildren().indexOf(v)<v.getParent().getChildren().size()-1) && (c.compare(x, v.getParent().getItems().get(v.getParent().getChildren().indexOf(v)))!=0)){
				System.out.println("Parent does not contain item for non-rightmost leaf: " + v.getParent());
				return false;
			}
			if(!(v.getParent().getChildren().indexOf(v)<v.getParent().getChildren().size()-1) && (v.getParent().getItems().contains(x))){
				System.out.println("Parent contains last leaf's item: " + v.getParent());
				return false;
			}
			if(v.getParent().getChildren().indexOf(v)==v.getParent().getChildren().size()-1){
				//now x should be in exactly one ancestor, the one at which the child leading to v is not the rightmost
				for(u=v.getParent(); (u!=root) && (u==u.getParent().getChildren().get(u.getParent().getChildren().size()-1)); u=u.getParent()){
					if(u.getItems().contains(x)){ //this would be too soon
						System.out.println("Leaf's item found in earlier ancestor: " + v + " " + u);
						return false;
					}
				}
				if(u==root){
					System.out.println("Went to root looking for non-greatest, x=" + x + "; greatest=" + greatest);
				}
				u=u.getParent();
				if(!u.getItems().contains(x)){
					System.out.println("Item not found in correct ancestor: " + v + " " + u);
					return false;
				}
				if(u==root) //ancestor was root, no need to look any higher
					return true;
				for(u=u.getParent(); true; u=u.getParent()){
					if(u.getItems().contains(x)){ //this would be too late
						System.out.println("Leaf's item found in higher ancestor: " + u);
						return false;
					}
					if(u==root) //made it to the right and item was not found again
						break;
				}
			}
			return true;
		}
		if(v.getItems().size()+1!=v.getChildren().size()){
			System.out.println("Node's number of items and children mismatch: " + v);
			return false;
		}
		for(int i=0; i<v.getChildren().size()-1; i++){ //for each child but the last
			u = v.getChildren().get(i);
			//find the greatest (right-most) descendant leaf of this child
			while(!u.getChildren().isEmpty())
				u = u.getChildren().get(u.getChildren().size()-1);
			if(c.compare(v.getItems().get(i), u.getItems().get(0))!=0){
				System.out.println("Node's items does not correspond to greatest: " + u.getItems().get(0) + " " + v);
				return false;
			}
		}
		//now check that v's items occur nowhere else higher in the tree
		if(v!=root){
			for(u=v.getParent(); u!=root; u=u.getParent()){
				for(T y: v.getItems()){
					if(u.getItems().contains(y)){
						System.out.println("Node's item found higher in tree: " + v + " " + u);
						return false;
					}
				}
			}
		}
		return true;
	}

	private boolean verifyRecursive(TwoFourNode<T> v) {
		if(!verifyNode(v))
			return false;
		for(TwoFourNode<T> u: v.getChildren()){
			if(!verifyRecursive(u))
				return false;
		}
		return true;
	}

	public boolean verify() {
		return verifyRecursive(root);
	}


//	//TESTING
//	public static void main(String[] args){
//		TwoFourTree<Integer> a = new TwoFourTree<Integer>();
//		for(int i=0; i<500; i++)
//			a.add(new Integer(((int)(1000*Math.random()))));
//		System.out.println("n=" + a.size());
//		System.out.println("Tree is correct. " + a.verify());
//		System.out.println("Removing.");
//		while(a.size()>20)
//			a.remove(new Integer(((int)(1000*Math.random()))));
//		System.out.println("n=" + a.size());
//		System.out.println("Tree is correct. " + a.verify());
//		System.out.println("a=" + a);
//
//		/*System.out.println("Copying to b:");
//		TwoFourTree<Integer> b = new TwoFourTree<Integer>();
//		for(Integer i: a){
//			b.add(i);
//		}
//		System.out.println("b=" + b);
//		System.out.println("Adding elements to a.");
//		for(int i=0; i<40; i++)
//			a.add(new Integer(((int)(50*Math.random()))));
//		System.out.println("a=" + a);
//		System.out.println("a.subsetOf(b): " + a.subsetOf(b));
//		System.out.println("b.subsetOf(a): " + b.subsetOf(a));
//		System.out.println("a.subsetOf(a): " + a.subsetOf(a));
//		System.out.println("b.subsetOf(b): " + b.subsetOf(b));*/
//
//		/*System.out.println("Making new set:");
//		TwoFourTree<Integer> b = new TwoFourTree<Integer>();
//		for(int i=0; i<20; i++)
//			b.add(new Integer(((int)(50*Math.random()))));
//		System.out.println("b=" + b);*/
//		/*a.intersectWith(b);
//		System.out.println("a.intersectWith(b)=" + a);*/
//		/*a.differenWith(b);
//		System.out.println("a\b=" + a);*/
//		/*a.unionWith(b);
//		System.out.println("a.unionWith(b)=" + a);*/
//
//		/*System.out.println("Removing.");
//		Iterator<Integer> it = a.iterator();
//		while(it.hasNext()){
//			it.next();
//			it.remove();
//			System.out.println("Tree is correct. " + a.verify());
//			System.out.println(a);
//		}*/
//
//		/*System.out.println("Adding.");
//		for(int i=0; i<500; i++)
//			a.add(new Integer(((int)(1000*Math.random()))));
//		System.out.println("n=" + a.size());
//		System.out.println("Tree is correct. " + a.verify());
//		System.out.println("Removing.");
//		for(int i=0; i<1000000; i++)
//			a.remove(new Integer(((int)(1000*Math.random()))));
//		System.out.println("n=" + a.size());
//		System.out.println("Tree is correct. " + a.verify());*/
////		for(Integer x: a)
////			a.remove(x);
//		//System.out.println(a.toString2());
//	}

}