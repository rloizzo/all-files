;; scheme homework 4 - daily 8
;; name: Ryan Loizzo
;; date: February 8

(include "paradigms_d8.scm")

(define filterN
  (lambda (n m lat)
    (cond
      ((null? lat) '())
      ((and (number? (car lat)) (> (car lat) m)) (filterN n m (cdr lat)))
      ((and (number? (car lat)) (< (car lat) n)) (filterN n m (cdr lat)))
      ((number? (car lat)) (cons (car lat) (filterN n m (cdr lat))))
    (else
      (filterN n m (cdr lat))))))  
    ;; currently this function just returns the lat as it is given
    ;; change the function so that it returns /only/ the numbers
    ;; >= n and <= m
    ;; see below for examples...

;; tests!
(display (filterN 4 6 '(1 turkey 5 9 4 bacon 6 cheese)))
(display "\n")

(display (filterN 4 6 '(4 4 4 1 1 bacon 9 9 8 6 6 6 1 4 5)))
(display "\n")

;; correct output:
;;   $ guile d8.scm
;;   (5 4 6)
;;   (4 4 4 6 6 6 4 5)

