var date = new Date();
var year = date.getFullYear() // 年
var month = date.getMonth() + 1; // 月
var day  = date.getDate(); // 日
var hour = date.getHours(); // 时
var minutes = date.getMinutes(); // 分
var seconds = date.getSeconds() //秒
var weekArr = ['星期日','星期一', '星期二', '星期三', '星期四', '星期五', '星期六' ];
var week = weekArr[date.getDay()];
// 给一位数数据前面加 “0”
if (month >= 1 && month <= 9) {
 month = "0" + month;
}
if (day >= 0 && day <= 9) {
 day = "0" + day;
}
if (hour >= 0 && hour <= 9) {
 hour = "0" + hour;
}
if (minutes >= 0 && minutes <= 9) {
 minutes = "0" + minutes;
}
if (seconds >= 0 && seconds <= 9) {
 seconds = "0" + seconds;
}
document.write("上次更新："+year + "年" + month + "月" + day + "日" + " " + week + " "+hour + ":" + minutes + ":"+ seconds);
//return currentdate;
