import bb.cascades 1.4

Page {
    id: root

    property string method
    property int period
    property alias stationaryDetection: statDetectCheckBox.checked

    titleBar: TitleBar {
        title: qsTr("Settings")
        visibility: ChromeVisibility.Visible
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        topPadding: ui.du(2.0)
        leftPadding: topPadding
        rightPadding: leftPadding
        bottomPadding: topPadding
        DropDown {
            title: qsTr("Positioning Method")
            options: [
                Option {
                    text: qsTr("All")
                },
                Option {
                    text: qsTr("Satellite")
                },
                Option {
                    text: qsTr("Non-Satellite")
                }
            ]
            onSelectedIndexChanged: {
                var opts = ["all", "satellite", "nonSatellite"]
                root.method = opts[selectedIndex]
            }
            selectedIndex: 0
        }
        DropDown {
            title: qsTr("Update Interval")
            options: [
                Option {
                    text: qsTr("1 s")
                },
                Option {
                    text: qsTr("2 s")
                },
                Option {
                    text: qsTr("5 s")
                },
                Option {
                    text: qsTr("10 s")
                },
                Option {
                    text: qsTr("30 s")
                },
                Option {
                    text: qsTr("1 min")
                }
            ]
            onSelectedIndexChanged: {
                var opts = [1, 2, 5, 10, 30, 60]
                root.period = opts[selectedIndex]
            }
            selectedIndex: 2
        }
        CheckBox {
            id: statDetectCheckBox
            text: qsTr("Stationary Detection")
        }
    }
}
