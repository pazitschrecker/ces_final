var load_question = function (q_id) {
  window.location.href = "http://127.0.0.1:5000/question/" + q_id + "";
};

// render feedback for questions
var render_feedback = function (q_id, feedback) {
  $(".feedback_section").val("");
  $(".feedback_section").append(feedback);
  $(".feedback_section").removeClass("hidden");
  $(".feedback_section").show();

  $("#next_btn_" + q_id).show();
};

var send_signal = function () {
  $.ajax({
    type: "POST",
    url: "send_signal",
    dataType: "json",
    contentType: "application/json; charset=utf-8",
    //data: JSON.stringify(data_to_save),
    success: function (result) {
      console.log("yay!!");
    },
    error: function (request, status, error) {
      console.log("Error");
      console.log(request);
      console.log(status);
      console.log(error);
    },
  });
};

var check_answer = function (q_id, curr_answer) {
  var data = { question: q_id, answer: curr_answer };
  $.ajax({
    type: "POST",
    url: "/check_answer",
    dataType: "json",
    contentType: "application/json; charset=utf-8",
    data: JSON.stringify(data),
    success: function (result) {
      var feedback = result["feedback"];
      var score = result["score"];
      render_feedback(q_id, feedback);
      //console.log(feedback);
    },
    error: function (request, status, error) {
      console.log("Error");
      console.log(request);
      console.log(status);
      console.log(error);
    },
  });
};

$(document).ready(function () {
  $("#next_btn_" + q_id).hide();
  $("#my_btn").click(function () {
    console.log("pressed");
    send_signal();
  });
  $("#start_btn").click(function () {
    console.log("load quiz");
    load_question(1);
  });
  $(".submit_answer_btn").click(function () {
    var q_id = parseInt($(this).attr("id").slice(-1));
    var ans = $("#submit_" + q_id)
      .val()
      .trim();
    check_answer(q_id, ans);
  });
  $(".next_q_btn").click(function () {
    var new_q_id = parseInt($(this).attr("id").slice(-1)) + 1;
    console.log("going to next question " + new_q_id);
    load_question(new_q_id);
  });
});
