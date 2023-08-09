import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Window {
    visible: true
    width: 800
    height: 600
    title: qsTr("视觉定位")
    color: "gray"


    property int column1Height: 30
    property int column1Width: 300
    property int labelWidth: 120
    Item{
        anchors.fill: parent

        Column{
            id:column1
            width:  column1Width
            spacing: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5

            Row{
                width: column1Width
                Label
                {
                    id:cameraLabel
                    text: '<font size="4" color="white">摄像头</font>'
                    verticalAlignment:Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    background: Rectangle
                    {
                        color:"#128bf1"
                    }
                    width: 60
                    height: column1Height
                }
                ComboBox{
                    id:cameraComboBox
                    model:CameraModelList
                    textRole: "description";
                    height: column1Height
                    width:column1Width-cameraLabel.width
                    onCurrentIndexChanged: {
                        CameraModelList.updataResolutions(parseInt(cameraComboBox.currentIndex))
                    }
                }
            }

            Row{
                width: column1Width
                Label
                {
                    id:resolutionLabel
                    text: '<font size="4" color="white">分辨率</font>'
                    verticalAlignment:Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    background: Rectangle
                    {
                        color:"#128bf1"
                    }
                    width: 60
                    height: column1Height
                }
                ComboBox{
                    id:resolutionComboBox
                    textRole: "resolution";
                    model:ResolutionModelList
                    width:column1Width-resolutionLabel.width
                    height: column1Height
                    onCountChanged: {
                        resolutionComboBox.currentIndex=resolutionComboBox.count-1;
                    }
                }
            }
            Row{
                width: column1Width
                Label
                {
                    id:thresholdTOZEROLabel
                    text: '<font size="4" color="white">二值化</font>'
                    verticalAlignment:Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    background: Rectangle
                    {
                        color:"#128bf1"
                    }
                    width: 60
                    height: column1Height
                }
                Slider{
                    id:thresholdTOZEROSlider
                    width:column1Width-thresholdTOZEROLabel.width-labelWidth
                    height: column1Height
                    from: 0
                    to:255
                    stepSize: 1
                    snapMode:Slider.SnapAlways
                    value:FFmpeg.thresholdTOZERO
                    onMoved: {
                        FFmpeg.thresholdTOZERO=parseInt(value)
                    }
                }
                Rectangle{
                    width: labelWidth
                    height: column1Height
                    Label
                    {
                        id:thresholdTOZEROValueLabel
                        text: parseInt(thresholdTOZEROSlider.value)
                        anchors.fill: parent
                        verticalAlignment:Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        background: Rectangle
                        {
                            color:"#128bf1"
                        }
                    }
                }
            }

            Row{
                id:cannyThresholdRow
                width: column1Width
                Label
                {
                    id:cannyLabel
                    text: '<font size="4" color="white">边缘检测</font>'
                    verticalAlignment:Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    background: Rectangle
                    {
                        color:"#128bf1"
                    }
                    width: 60
                    height: 60
                }
                RangeSlider{
                    id:cannyThresholdRange
                    width:column1Width-cannyLabel.width-labelWidth
                    from: 0
                    to:255
                    stepSize: 1.0
                    first.value:FFmpeg.firstCannythreshold
                    second.value: FFmpeg.secondCannythreshold
                    anchors.verticalCenter:cannyThresholdRow.verticalCenter
                    first.onMoved: {
                        FFmpeg.firstCannythreshold=parseInt(first.value)
                        cannyfirstThresholdLabel.text="弱边缘："+parseInt(first.value)
                    }
                    second.onMoved: {
                        FFmpeg.secondCannythreshold=parseInt(second.value)
                        cannysecondThresholdLabel.text="强边缘："+parseInt(second.value)
                    }
                }
                ColumnLayout{
                    width: labelWidth
                    height: 60
                    Rectangle{
                        Layout.alignment: Qt.AlignTop
                        Layout.fillWidth: true
                        height: 30
                        Label{
                            id:cannyfirstThresholdLabel
                            text: "弱边缘："+FFmpeg.firstCannythreshold
                            anchors.fill: parent
                            verticalAlignment:Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            background: Rectangle
                            {
                                color:"#128bf1"
                            }
                        }
                    }
                    Rectangle{
                        Layout.alignment: Qt.AlignTop
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Label{
                            id:cannysecondThresholdLabel
                            text: "强边缘："+FFmpeg.secondCannythreshold
                            anchors.fill: parent
                            verticalAlignment:Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            background: Rectangle
                            {
                                color:"#128bf1"
                            }
                        }
                    }
                }
            }
            Row{
                width: column1Width
                Label
                {
                    id:proportionLabel
                    text: '<font size="4" color="white">比例</font>'
                    verticalAlignment:Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    background: Rectangle
                    {
                        color:"#128bf1"
                    }
                    width: 60
                    height: column1Height
                }
                Slider{
                    id:proportionSlider
                    width:column1Width-thresholdTOZEROLabel.width-labelWidth
                    height: column1Height
                    from: 0.001
                    to:1
                    stepSize: 0.001
                    snapMode:Slider.NoSnap
                    value:FFmpeg.proportion
                    onMoved: {
                        FFmpeg.proportion=parseFloat(value.toFixed(3))
                    }
                }
                Rectangle{
                    width: labelWidth
                    height: column1Height
                    Label
                    {
                        id:proportionValueLabel
                        text:"长度(mm)/像素："+ parseFloat(proportionSlider.value.toFixed(3))
                        anchors.fill: parent
                        verticalAlignment:Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        background: Rectangle
                        {
                            color:"#128bf1"
                        }
                    }
                }
            }

            Row{
                id:areaThresholdRow
                width: column1Width
                Label
                {
                    id:areaLabel
                    text: '<font size="4" color="white">面积范围</font>'
                    verticalAlignment:Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    background: Rectangle
                    {
                        color:"#128bf1"
                    }
                    width: 60
                    height: 60
                }
                RangeSlider{
                    id:areaThresholdRange
                    width:column1Width-cannyLabel.width-labelWidth
                    from: 0
                    to:65555
                    stepSize: 1.0
                    first.value:FFmpeg.minArea
                    second.value: FFmpeg.maxArea
                    anchors.verticalCenter:areaThresholdRow.verticalCenter
                    first.onMoved: {
                        FFmpeg.minArea=parseInt(first.value)
                        minAreaLabel.text="最小面积："+parseInt(first.value)+"mm²"
                    }
                    second.onMoved: {
                        FFmpeg.maxArea=parseInt(second.value)
                        maxAreaLabel.text="最大面积："+parseInt(second.value)+"mm²"
                    }
                }
                ColumnLayout{
                    width: labelWidth
                    height: 60
                    Rectangle{
                        Layout.alignment: Qt.AlignTop
                        Layout.fillWidth: true
                        height: 30
                        Label{
                            id:minAreaLabel
                            text: "最小面积："+FFmpeg.minArea+"mm²"
                            anchors.fill: parent
                            verticalAlignment:Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            background: Rectangle
                            {
                                color:"#128bf1"
                            }
                        }
                    }
                    Rectangle{
                        Layout.alignment: Qt.AlignTop
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Label{
                            id:maxAreaLabel
                            text: "最大面积："+FFmpeg.maxArea+"mm²"
                            anchors.fill: parent
                            verticalAlignment:Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            background: Rectangle
                            {
                                color:"#128bf1"
                            }
                        }
                    }
                }
            }

            Button{
                id:openCameraButton
                text: '<font size="4" color="green">打开摄像头</font>'
                height: column1Height
                width: column1Width
                onClicked: {
                    //var temp= CameraModelList.data(CameraModelList.index(cameraComboBox.currentIndex,0),1);
                    //console.log("currentIndex",cameraComboBox.currentIndex)
                    //console.log("camera:",temp);
                    //console.log("resolution:",resolutionComboBox.currentText)
                    FFmpeg.init(cameraComboBox.currentText,resolutionComboBox.currentText);
                    FFmpeg.start();
                    openCameraButton.enabled=false;
                    resolutionComboBox.enabled=false;
                    cameraComboBox.enabled=false;
                }
            }
            Button{
                id:closeCameraButton
                text: '<font size="4" color="green">停止</font>'
                height: column1Height
                width: column1Width
                onClicked: {
                    FFmpeg.stop()
                    openCameraButton.enabled=true;
                    resolutionComboBox.enabled=true;
                    cameraComboBox.enabled=true;
                }
            }
            Button{
                id:saveConfigButton
                text: '<font size="4" color="green">保存当前配置</font>'
                height: column1Height
                width: column1Width
                onClicked: {
                    dialog.open()
                }
            }
        }
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.left: column1.right
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            color: "black"

            ColumnLayout{
                anchors.fill: parent
                Rectangle{
                    Layout.alignment: Qt.AlignTop
                    Layout.margins: 0
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    color: "black"
                    Image {
                        id:simage
                        anchors.fill: parent
                        anchors.margins: 3
                        cache:false
                        fillMode: Image.PreserveAspectFit
                    }
                }
                Rectangle
                {
                    color: "black"
                    border.color: "gray"
                    Layout.alignment: Qt.AlignTop
                    Layout.margins: 3
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                    Layout.maximumHeight: 80
                    Layout.minimumHeight: 80
                    ScrollView{
                        id:messageScrollView
                        anchors.fill: parent
                        TextArea{
                            id:messageTextArea
                            color: "green"
                            anchors.fill: parent
                            text: "运行日志"
                        }
                    }
                }
            }
        }
    }
    Dialog{
        id:dialog
        title: qsTr("确认保存当前配置吗？")
        modal:true
        closePolicy: Popup.NoAutoClose
        anchors.centerIn: Overlay.overlay
        implicitHeight: 200
        implicitWidth: 300
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: FFmpeg.saveOpencvConfig()
        onRejected: dialog.close()
    }

    Connections{
        target: FFmpeg
        function onEmitImage(){
            simage.source=""
            simage.source="image://imageprovider"
        }
    }
    Connections{
        target: FFmpeg
        function onRunLog(runlog){
            if(messageTextArea.length>100)
                messageTextArea.text=""
            messageTextArea.append(qsTr(runlog))
        }
    }
}
