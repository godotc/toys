'use struct';


const MESSAGE_TYPE_OFFER = 0x01;
const MESSAGE_TYPE_ANSWER = 0x02;
const MESSAGE_TYPE_CANDIDATE = 0x03;
const MESSAGE_TYPE_HANGUP = 0x04;


const SIGNAL_TYPE_JOIN = "join"; // 主动加入房间
const SIGNAL_TYPE_RESP_JOIN = "resp-join"; // 告知加入者对方是谁
const SIGNAL_TYPE_LEAVE = "leave"; // 主动离开房间

const SIGNAL_TYPE_NEW_PEER = "new-peer"; //有新人加入,通知所有在房间的人
const SIGNAL_TYPE_LEVEL = "peer-level"; // 一个人离开,notify all

const SIGNAL_TYPE_OFFER = "offer"; // 发送 offer 给对方 peer
const SIGNAL_TYPE_ANSWER = "answer";
const SIGNAL_TYPE_CANDIDATE = "candidate"; //



var localUserId = Math.random().toString(36).substring(2);
var remoteUserId = -1;
var localStream;
var pc = null;
var isInitor = false;




