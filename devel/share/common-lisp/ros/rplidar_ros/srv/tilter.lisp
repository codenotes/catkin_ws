; Auto-generated. Do not edit!


(cl:in-package rplidar_ros-srv)


;//! \htmlinclude tilter-request.msg.html

(cl:defclass <tilter-request> (roslisp-msg-protocol:ros-message)
  ((command
    :reader command
    :initarg :command
    :type cl:string
    :initform "")
   (argument
    :reader argument
    :initarg :argument
    :type cl:float
    :initform 0.0))
)

(cl:defclass tilter-request (<tilter-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <tilter-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'tilter-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name rplidar_ros-srv:<tilter-request> is deprecated: use rplidar_ros-srv:tilter-request instead.")))

(cl:ensure-generic-function 'command-val :lambda-list '(m))
(cl:defmethod command-val ((m <tilter-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rplidar_ros-srv:command-val is deprecated.  Use rplidar_ros-srv:command instead.")
  (command m))

(cl:ensure-generic-function 'argument-val :lambda-list '(m))
(cl:defmethod argument-val ((m <tilter-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rplidar_ros-srv:argument-val is deprecated.  Use rplidar_ros-srv:argument instead.")
  (argument m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <tilter-request>) ostream)
  "Serializes a message object of type '<tilter-request>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'command))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'command))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'argument))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <tilter-request>) istream)
  "Deserializes a message object of type '<tilter-request>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'command) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'command) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'argument) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<tilter-request>)))
  "Returns string type for a service object of type '<tilter-request>"
  "rplidar_ros/tilterRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'tilter-request)))
  "Returns string type for a service object of type 'tilter-request"
  "rplidar_ros/tilterRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<tilter-request>)))
  "Returns md5sum for a message object of type '<tilter-request>"
  "1295ef2f98a07017591561c04a6e6c75")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'tilter-request)))
  "Returns md5sum for a message object of type 'tilter-request"
  "1295ef2f98a07017591561c04a6e6c75")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<tilter-request>)))
  "Returns full string definition for message of type '<tilter-request>"
  (cl:format cl:nil "string command~%float32 argument~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'tilter-request)))
  "Returns full string definition for message of type 'tilter-request"
  (cl:format cl:nil "string command~%float32 argument~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <tilter-request>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'command))
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <tilter-request>))
  "Converts a ROS message object to a list"
  (cl:list 'tilter-request
    (cl:cons ':command (command msg))
    (cl:cons ':argument (argument msg))
))
;//! \htmlinclude tilter-response.msg.html

(cl:defclass <tilter-response> (roslisp-msg-protocol:ros-message)
  ((result
    :reader result
    :initarg :result
    :type cl:string
    :initform ""))
)

(cl:defclass tilter-response (<tilter-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <tilter-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'tilter-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name rplidar_ros-srv:<tilter-response> is deprecated: use rplidar_ros-srv:tilter-response instead.")))

(cl:ensure-generic-function 'result-val :lambda-list '(m))
(cl:defmethod result-val ((m <tilter-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rplidar_ros-srv:result-val is deprecated.  Use rplidar_ros-srv:result instead.")
  (result m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <tilter-response>) ostream)
  "Serializes a message object of type '<tilter-response>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'result))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'result))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <tilter-response>) istream)
  "Deserializes a message object of type '<tilter-response>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'result) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'result) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<tilter-response>)))
  "Returns string type for a service object of type '<tilter-response>"
  "rplidar_ros/tilterResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'tilter-response)))
  "Returns string type for a service object of type 'tilter-response"
  "rplidar_ros/tilterResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<tilter-response>)))
  "Returns md5sum for a message object of type '<tilter-response>"
  "1295ef2f98a07017591561c04a6e6c75")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'tilter-response)))
  "Returns md5sum for a message object of type 'tilter-response"
  "1295ef2f98a07017591561c04a6e6c75")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<tilter-response>)))
  "Returns full string definition for message of type '<tilter-response>"
  (cl:format cl:nil "string result~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'tilter-response)))
  "Returns full string definition for message of type 'tilter-response"
  (cl:format cl:nil "string result~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <tilter-response>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'result))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <tilter-response>))
  "Converts a ROS message object to a list"
  (cl:list 'tilter-response
    (cl:cons ':result (result msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'tilter)))
  'tilter-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'tilter)))
  'tilter-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'tilter)))
  "Returns string type for a service object of type '<tilter>"
  "rplidar_ros/tilter")