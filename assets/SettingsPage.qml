import bb.cascades 1.3

Page {
    id: root

    property int period: 5
    property alias stationaryDetection: stationaryDetection.checked

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
        LabelToggleButton {
            id: stationaryDetection
            text: qsTr("Stationary detection")
        }
    }
}
