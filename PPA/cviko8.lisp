(defun my-del-val (lst val)
	(if (null lst)
		nil
		(if (equal val (car lst)) 
			(my-del-val (cdr lst) val);kdybych chtel abych smazal jen prvni vyskyt tak bych tento radek nahradil (cdr lst) 
			(cons (car lst) (my-del-val (cdr lst) val))
		)
	)
)

;reprezentace bvs(bst binary search tree) v lispu
;(val leftch rightch)
;napr.: (5 (3 nil nil) nil) -- 5 v rootu, nema pravyho childa, leveho childa ma trojku 

(defun my-append (l1 l2)
	(if (null l1)
		(if (null l2)
			nil	
			(my-append l2 nil)
		)		
		(cons (car l1) (my-append (cdr l1) l2))
	)
)

(defun bstL (lst)
	;(cadr lst)
	(car (cdr lst))
)

(defun bstR (lst)
	;(caddr lst)
	(car (cdr (cdr lst)))
)

(defun print-preorder-bst (bst)
	(if (null (car bst))
		nil
		(my-append 
			(cons (car bst) (print-preorder-bst (bstL bst)))
			(print-preorder-bst (bstR bst))
		)
	)	
)

(defun find-in-bst (bst val)
	(cond
		((null bst) nil)
		((equal val (car bst)) T)
		((< val (car bst)) (find-in-bst (bstL bst) val))
		((> val (car bst)) (find-in-bst (bstR bst) val))
	)
)

(defun bst-height (bst) 
	(if (null bst)
		-1
		(+ 1 (max (bst-height (bstL bst)) (bst-height (bstR bst))))
	)
)

(defun my-length (lst)
	(if (null lst)
		0
		(+ 1 (my-length (cdr lst)))
	)
)

;matice '((1 2 3) (4 5 6) (7 8 9) (10 11 12))
(defun mat-dimension (matice)
	(if (null matice)
		(cons 0 0)
		(cons 
			(my-length matice)
			(my-length (car matice))
		)
	)
)

(defun my-nth-elem (lst n)
	(if (equal n 1)
		(car lst)
		(my-nth-elem (cdr lst) (- n 1))
	)
)

(defun mat-nth-col (mat n)
	;(mapcar (lambda (row) (my-nth-elem row n)) mat) ;nejakej hack
	(if (null mat)
		nil
		(cons (my-nth-elem (car mat) n) (mat-nth-col (cdr mat) n))
	)
)

;wtfff
(defun dfs-all-n (g visited neighbours)
	(if (null neighbours)
		nil
		(progn
			(cons (car neighbours) visited)
			(dfs g (car neighbour) visited)
			(dfs-all-n g visited (cdrr neighbours))
		)
	)
)

(defun dfs (g node &optional (visited nil))
	(cond
		((member node visited) nil)
		(dfs-all-n g visited (neighbours g node))
	)		
)

(defun neighbours (g node)
	(let ((edges (cadr g)))
		(mapcar
			(lambda (pair) (xyz node pair))
			edges	
		)	
	)
)

(defun xyz (node pair)
	(cond
		((equal node (car pair)) (cadr pair))
		((equal nodde (cadr pair)) (car pair))
		(T nil)
	)
)		
;----------------------------------------------

(let ((bst '(5 (4 nil nil) (36 nil (5 nil (23 nil nil))))))
	(print (print-preorder-bst bst))
)

;graf k prochazeni
(let ((g '( (A B C D E) ((A B) (B C) (C A) (B D))))))
	(dfs 'A g)
