import bb.cascades 1.4
import bb.system 1.0
import a12n.geoPosition 0.1
import a12n.gpx 0.1

NavigationPane {
    id: navPane

    attachedObjects: [
        SettingsPage {
            id: settingsPage
        }
    ]

    Menu.definition: MenuDefinition {
        helpAction: HelpActionItem {
        }
        settingsAction: SettingsActionItem {
            onTriggered: {
                navPane.push(settingsPage)
            }
        }
    }

    Page {
        id: mainPage

        function dateTimeDifference(beginTs, endTs) {
            var diff = endTs.valueOf() - beginTs.valueOf()
            var ans = {}
            ans.days = ~~(diff / 86400000)
            diff %= 86400000
            ans.hours = ~~(diff / 3600000)
            diff %= 3600000
            ans.minutes = ~~(diff / 60000)
            diff %= 60000
            ans.seconds = ~~(diff / 1000)
            diff %= 1000
            ans.milliseconds = diff
            return ans
        }

        function durationString(beginTs, endTs) {
            var d = dateTimeDifference(beginTs, endTs)
            var ans = ""
            if (d.days > 0) {
                ans += d.days + "d"
            }
            if (d.hours > 0) {
                ans += d.hours + "h"
            }
            if (d.minutes > 0) {
                ans += d.minutes + "min"
            }
            ans += d.seconds + "s"
            return ans
        }

        attachedObjects: [
            GeoPositionSource {
                id: posSrc
                active: true
                provider: settingsPage.provider
                period: settingsPage.period
                stationaryDetectionEnabled: settingsPage.stationaryDetection
                onPositionChanged: {
                    latLabel.text = latitude.toFixed(6) + "°"
                    lonLabel.text = longitude.toFixed(6) + "°"
                    altLabel.text = altitude.toFixed(2) + " m"
                    gpx.writeTrackPoint(timestamp, latitude, longitude, altitude)
                }
            },
            GpxFileWriter {
                id: gpx
                open: false
                function generateFileName(startTime) {
                    return Qt.formatDateTime(startTime, "yyyyMMdd_HHmmss") + ".gpx"
                }
            },
            SystemToast {
                id: saveToast
            }
        ]

        actions: [
            ActionItem {
                id: startAction
                title: qsTr("Record")
                ActionBar.placement: ActionBarPlacement.Signature
                enabled: true
                onTriggered: {
                    gpx.fileName = gpx.generateFileName(new Date)
                    gpx.open = true
                    startAction.enabled = false
                    stopAction.enabled = true
                }
                imageSource: "asset:///images/ic_location.png"
            },
            ActionItem {
                id: stopAction
                title: qsTr("Stop")
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: false
                onTriggered: {
                    gpx.open = false
                    saveToast.body = gpx.fileName
                    saveToast.show()
                    startAction.enabled = true
                    stopAction.enabled = false
                }
                imageSource: "asset:///images/ic_stop.png"
            }
        ]

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            topPadding: ui.du(2.0)
            leftPadding: topPadding
            rightPadding: leftPadding
            bottomPadding: topPadding
            Label {
                id: latLabel
                text: "N/A°"
                textStyle.fontSize: FontSize.XXLarge
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.textAlign: TextAlign.Center
                textStyle.fontFamily: "Monospace"
            }
            Label {
                id: lonLabel
                text: "N/A°"
                textStyle.fontSize: FontSize.XXLarge
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.textAlign: TextAlign.Center
                textStyle.fontFamily: "Monospace"
            }
            Label {
                id: altLabel
                text: "N/A m"
                textStyle.fontSize: FontSize.Large
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.fontFamily: "Monospace"
            }
            Divider {
                horizontalAlignment: HorizontalAlignment.Fill
            }
            Label {
                // TODO: update
                id: trackTimeLabel
                text: "00:00:00"
                textStyle.fontSize: FontSize.XXLarge
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.textAlign: TextAlign.Center
            }
        }
    }
}