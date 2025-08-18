$(function () {
    // 进入页面时显示数据
    showLocalDataToHtml();
    // 输入框输入完成后按回车键
    $("#title").on("keydown", function (event) {
        if (event.keyCode === 13) {
            // alert(11)
            if ($(this).val() === "") {
                alert("请输入您要的操作事项！")
                return;
            }
            var localdate = getLocalData();
            localdate.push({ title: $(this).val(), done: false })
            saveLocalData(localdate);
            showLocalDataToHtml();
            $(this).val("");
        }

    })
    // 删除按钮
    $("ol, ul").on("click", "a", function () {
        var localdata = getLocalData();
        var index = $(this).attr("id");
        // console.log(index);
        localdata.splice(index, 1);
        saveLocalData(localdata);
        showLocalDataToHtml();
    })

    // 复选框的选中处理
    $("ol, ul").on("click", "input", function () {
        // alert(1)
        var localdata = getLocalData();
        var index = $(this).siblings("a").attr("id");
        // console.log(index);
        localdata[index].done = $(this).prop('checked');
        // console.log(localdata);
        saveLocalData(localdata);
        showLocalDataToHtml();

    })
    function getLocalData() {
        var localdate = localStorage.getItem("todolist");
        if (localdate !== null) {
            return JSON.parse(localdate);
        }
        else {
            return [];
        }
    }

    function saveLocalData(localdate) {
        localStorage.setItem("todolist", JSON.stringify(localdate));
    }

    function showLocalDataToHtml() {
        $("#todolist, #donelist").empty();
        var todoCount = 0;
        var doneCount = 0;
        var localdate = getLocalData();
        $.each(localdate, function (i, n) {
            if (n.done) {
                doneCount ++;
                $("ul").prepend("<li><input type='checkbox' checked='checked'> <p>" + n.title + "</p> <a href='javascript:;' id=" + i + ">删除</a> </li>");
            }
            else {
                todoCount ++;
                $("ol").prepend("<li><input type='checkbox'> <p>" + n.title + "</p> <a href='javascript:;' id=" + i + ">删除</a> </li>");

            }
        })
        $("#todocount").text(todoCount);
        $("#donecount").text(doneCount);
    }

})
