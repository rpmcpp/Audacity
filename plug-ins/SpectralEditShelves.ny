;nyquist plugin
;version 4
;type process
;preview enabled
;name "Spectral edit shelves..."
;action "Filtering..."
;author "Paul Licameli"
;copyright "Released under terms of the GNU General Public License version 2"


;; SpectralEditShelves.ny by Paul Licameli, November 2014.
;; Updated to version 4 by Steve Daulton November 2014.
;; Released under terms of the GNU General Public License version 2:
;; http://www.gnu.org/licenses/old-licenses/gpl-2.0.html

;control control-gain "Gain (dB)" real "" 0 -24 24

(setf control-gain (min 24 (max -24 control-gain))) ; excessive settings may crash

(defun mid-shelf (sig lf hf gain)
  "Combines high shelf and low shelf filters"
  (let* ((invg (- gain)))
    (scale (db-to-linear gain)
           (eq-highshelf (eq-lowshelf sig lf invg)
                         hf invg))))

(defun wet (sig gain)
  (let ((f0 (get '*selection* 'low-hz))
        (f1 (get '*selection* 'high-hz)))
    (cond
     ((not (or f0 f1))
        (throw 'debug-message (format nil "~aPlease select frequencies." p-err)))
     ((not f0) (eq-lowshelf sig f1 gain))
     ((not f1) (eq-highshelf sig f0 gain))
     (t (mid-shelf sig f0 f1 gain)))))

(defun result (sig)
  (let*
      ((tn (truncate len))
       (rate (snd-srate sig))
       (transition (truncate (* 0.01 rate)))  ; 10 ms
       (t1 (min transition (/ tn 2)))         ; fade in length (samples)
       (t2 (max (- tn transition) (/ tn 2)))  ; length before fade out (samples)
       (breakpoints (list t1 1.0 t2 1.0 tn))
       (env (snd-pwl 0.0 rate breakpoints)))
    (sum (prod env (wet sig control-gain)) (prod (diff 1.0 env) sig))))

(cond
  ((not (get '*TRACK* 'VIEW)) ; 'View is NIL during Preview
      (setf p-err (format nil "This effect requires a frequency selction in the~%~
                              'Spectrogram' or 'Spectrogram (log f)' track view.~%~%"))
      (catch 'debug-message
        (multichan-expand #'result *track*)))
  ((string-not-equal (get '*TRACK* 'VIEW) "spectrogram"  :end1 4 :end2 4)
      "Use this effect in the 'Spectrogram'\nor 'Spectrogram (log f)' view.")
  (T  (setf p-err "")
      (if (= control-gain 0)  ; Allow dry preview
          "Gain is zero. Nothing to do."
          (catch 'debug-message
            (multichan-expand #'result *track*)))))

