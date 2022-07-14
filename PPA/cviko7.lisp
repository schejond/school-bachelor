; 1 2 3 -> 1 e 2 e 3
(defun vloz (lst e)
	(if (null (cdr lst))
		lst
		(cons (car lst) (cons e (vloz (cdr lst) e)))
	)
)


(defun vloz2 (e lst &optional acc)
	(if (null lst)
		(reverse (cdr acc))
		(vloz2 e (cdr lst) (cons e (cons (car lst) acc)))))

;(my-flatten '(1 (2 3 4) (5 (6 7)) (8 9)))
(defun my-flatten (lst &optional(acc nil))
	(if (null lst) 
		acc
		(if (atom (car lst))
			(my-flatten (cdr lst) (append acc (list (car lst) ) ) )
			(my-flatten (cdr lst) (append acc (my-flatten (car lst))))
		)
	)
);todo

;udelej potencni mnozinu listu
(defun powerset (lst)
	(if (null lst);tzn ze ma velikost jedna
		(cons nil nil)
		(if (null (cdr lst))
			(cons (cons (car lst) nil)
				(cons nil nil)
			)
			(append
				(powerset (cdr lst))
				(f (powerset (cdr lst)) (car lst))
			)
		)
	)
);todo

(defun my-mergeSort (lst)
	
)
(defun my-split (lst)
	(my-split2 lst (/ (length lst) 2))
)

(defun my-split2 (lst stop &optional (i 1) a1 a2)
	(if (null lst)
		(cons a1 a2)
		(if (<= i stop)
			(my-split2 (cdr lst) stop (+ 1 i) (cons (car lst) a1) a2)
			(my-split2 (cdr lst) stop (+ 1 i) a1 (cons (car lst) a2))
		)	
	)	
)
(defun my-merge (l1 l2)
	(cond
		((null l1) ())
		((null l2) ())
		((< (car l1) (car l2)) (cons (car l1) (my-merge(cdr l1) l2)))
		(T (cons (car l2) (my-merge l1 (cdr l2))))
	)
)

