;; Auto-generated. Do not edit!


(when (boundp 'rplidar_ros::tilter)
  (if (not (find-package "RPLIDAR_ROS"))
    (make-package "RPLIDAR_ROS"))
  (shadow 'tilter (find-package "RPLIDAR_ROS")))
(unless (find-package "RPLIDAR_ROS::TILTER")
  (make-package "RPLIDAR_ROS::TILTER"))
(unless (find-package "RPLIDAR_ROS::TILTERREQUEST")
  (make-package "RPLIDAR_ROS::TILTERREQUEST"))
(unless (find-package "RPLIDAR_ROS::TILTERRESPONSE")
  (make-package "RPLIDAR_ROS::TILTERRESPONSE"))

(in-package "ROS")





(defclass rplidar_ros::tilterRequest
  :super ros::object
  :slots (_command _argument ))

(defmethod rplidar_ros::tilterRequest
  (:init
   (&key
    ((:command __command) "")
    ((:argument __argument) 0.0)
    )
   (send-super :init)
   (setq _command (string __command))
   (setq _argument (float __argument))
   self)
  (:command
   (&optional __command)
   (if __command (setq _command __command)) _command)
  (:argument
   (&optional __argument)
   (if __argument (setq _argument __argument)) _argument)
  (:serialization-length
   ()
   (+
    ;; string _command
    4 (length _command)
    ;; float32 _argument
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _command
       (write-long (length _command) s) (princ _command s)
     ;; float32 _argument
       (sys::poke _argument (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _command
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _command (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; float32 _argument
     (setq _argument (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(defclass rplidar_ros::tilterResponse
  :super ros::object
  :slots (_result ))

(defmethod rplidar_ros::tilterResponse
  (:init
   (&key
    ((:result __result) "")
    )
   (send-super :init)
   (setq _result (string __result))
   self)
  (:result
   (&optional __result)
   (if __result (setq _result __result)) _result)
  (:serialization-length
   ()
   (+
    ;; string _result
    4 (length _result)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _result
       (write-long (length _result) s) (princ _result s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _result
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _result (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass rplidar_ros::tilter
  :super ros::object
  :slots ())

(setf (get rplidar_ros::tilter :md5sum-) "1295ef2f98a07017591561c04a6e6c75")
(setf (get rplidar_ros::tilter :datatype-) "rplidar_ros/tilter")
(setf (get rplidar_ros::tilter :request) rplidar_ros::tilterRequest)
(setf (get rplidar_ros::tilter :response) rplidar_ros::tilterResponse)

(defmethod rplidar_ros::tilterRequest
  (:response () (instance rplidar_ros::tilterResponse :init)))

(setf (get rplidar_ros::tilterRequest :md5sum-) "1295ef2f98a07017591561c04a6e6c75")
(setf (get rplidar_ros::tilterRequest :datatype-) "rplidar_ros/tilterRequest")
(setf (get rplidar_ros::tilterRequest :definition-)
      "string command
float32 argument
---
string result

")

(setf (get rplidar_ros::tilterResponse :md5sum-) "1295ef2f98a07017591561c04a6e6c75")
(setf (get rplidar_ros::tilterResponse :datatype-) "rplidar_ros/tilterResponse")
(setf (get rplidar_ros::tilterResponse :definition-)
      "string command
float32 argument
---
string result

")



(provide :rplidar_ros/tilter "1295ef2f98a07017591561c04a6e6c75")


