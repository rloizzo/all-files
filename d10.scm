;; scheme 6 - daily homework 10
;; name: Ryan Loizzo
;; date: February 13, 2018

;; notice the use of debugging traps finally!!
;;(use-modules (ice-9 debugging traps) (ice-9 debugging trace))



(define sum*
  (lambda (ttup)
    (cond
	  ((null? ttup) 0)
      ((null? (cdr ttup)) (car (car ttup)))
      (else (+ (sum* (car (cdr ttup))) (sum* (cons (car ttup) (cdr (cdr ttup)))))))))

;;(install-trap (make <procedure-trap>
  ;;                          #:procedure sum*
    ;;                        #:behaviour (list trace-trap trace-until-exit)))

;; tests!
(display (sum* '((5)) ))
(display "\n")

(display (sum* '((0) ((0) ((5))) ((0) ((10)))) ))
(display "\n")

(display (sum* '((0) ((0) ((5) ((7)))) ((0) ((10) ))) ))
(display "\n")

(display (sum* '((0) ((0) ((5) ((7) ) ((8) ))) ((0) ((10) ))) ))
(display "\n")

;; correct output:
;;   $ guile d10.scm
;;   5
;;   15
;;   22
;;   30

