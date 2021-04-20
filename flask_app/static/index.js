var load_question = function (q_id) {
  window.location.href = "http://127.0.0.1:5000/question/" + q_id + "";
};

var finish_quiz = function (q_id) {
  window.location.href = "http://127.0.0.1:5000/done";
};

// render feedback for questions
var render_feedback = function (q_id, feedback, correct) {
  $(".feedback_section").val("");
  $(".feedback_section").append(feedback);
  $(".feedback_section").removeClass("hidden");
  $(".feedback_section").show();

  if (correct == true) $("#answer_area_" + q_id).addClass("lightgreen");
  else $("#answer_area_" + q_id).addClass("lightred");

  if (q_id < 5) $("#next_btn_" + q_id).show();
  //questions 1-4 have next question button
  else $("#finish_btn").show(); //finish quiz after question 5
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
      var correct = result["is_correct"];
      render_feedback(q_id, feedback, correct);
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
  $("#finish_btn").hide();
  $("#my_btn").click(function () {
    send_signal();
  });

  $("#start_btn").click(function () {
    load_question(1);
  });

  $(".submit_answer_btn").click(function (e) {
    var id = $(this).attr("id");
    $("#" + id).attr("disabled", true);
    var q_id = parseInt(id.slice(-1));
    var ans = $("#answer_" + q_id).val();
    check_answer(q_id, ans);
  });

  $(".next_q_btn").click(function () {
    var new_q_id = parseInt($(this).attr("id").slice(-1)) + 1;
    load_question(new_q_id);
  });

  $("#finish_btn").click(function () {
    finish_quiz();
  });
});
