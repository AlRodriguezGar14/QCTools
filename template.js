const CONSTANTS = Object.freeze({
  CYAN: [80, 140, 155],
})

window.addEventListener("DOMContentLoaded", () => {
  let video = document.getElementById("video")
  let playBtn = document.getElementById("play")
  let lastBtn = document.getElementById("last90")
  let progressOverlay = document.getElementById("progress-overlay")
  let seekOverlay = document.getElementById("seek-overlay")
  let errorsBar = document.getElementById("errors-bar")
  let dpfButtons = document.getElementsByClassName("dpf")
  let ipfButtons = document.getElementsByClassName("ipf")
  let playSpeed = document.getElementById("play-speed")
  let currentFrameElement = document.getElementById("currentFrame")
  let currentTimecodeElement = document.getElementById("currentTimecode")
  let getTimecode = document.getElementById("get-timecode")
  let getFCPXTimecode = document.getElementById("get-fcpx-timecode")
  let fps = 23.976024
  let totalFrames

  function frameToTimecode(frame, fps) {
    const timeInSeconds = frame / fps
    const hours = Math.floor(timeInSeconds / 3600)
    const minutes = Math.floor((timeInSeconds - hours * 3600) / 60)
    const seconds = Math.floor(timeInSeconds % 60)
    const milliseconds = Math.round(
      (timeInSeconds - Math.floor(timeInSeconds)) * 1000
    )
    return `${hours.toString().padStart(2, "0")}:${minutes
      .toString()
      .padStart(2, "0")}:${seconds.toString().padStart(2, "0")},${milliseconds
      .toString()
      .padStart(2, "0")}`
  }

  function frameToFCPXTimecode(frame, fps) {
    const adjustedFps = fps === 23.976 ? 23.98 : fps
    const correctionFactor = 1.001
    const correctedFrame = frame / correctionFactor
    const hours = Math.floor(correctedFrame / (3600 * adjustedFps))
    const minutes = Math.floor(
      (correctedFrame % (3600 * adjustedFps)) / (60 * adjustedFps)
    )
    const seconds = Math.floor(
      (correctedFrame % (60 * adjustedFps)) / adjustedFps
    )
    const frames = Math.floor(correctedFrame % adjustedFps)
    return `${hours.toString().padStart(2, "0")}:${minutes
      .toString()
      .padStart(2, "0")}:${seconds.toString().padStart(2, "0")}:${frames
      .toString()
      .padStart(2, "0")}`
  }

  function paintError(frame, error, color) {
    if (!totalFrames) return
    const positionPercent = (frame / totalFrames) * 100
    const errorElement = document.createElement("div")
    errorElement.style.position = "absolute"
    errorElement.style.left = `${positionPercent}%`
    errorElement.style.top = "0"
    errorElement.style.width = "2px"
    errorElement.style.height = "100%"
    errorElement.style.backgroundColor = color
    errorsBar.appendChild(errorElement)
  }

  function processButtons(buttons, errorType) {
    Array.from(buttons).forEach((button) => {
      const frameMatch = button.textContent.match(/frame: (\d+)/)
      if (frameMatch) {
        const frame = parseInt(frameMatch[1], 10)
        button.onclick = (event) => seek(event, frame)
        switch (errorType) {
          case "dpf":
            button.style.backgroundColor = "#973131"
            break
          case "ipf":
            button.style.backgroundColor = "#597445"
            break
        }
        paintError(frame, errorType, button.style.backgroundColor)
      }
    })
  }

  video.addEventListener("loadedmetadata", function () {
    totalFrames = Math.round(video.duration * fps)
    processButtons(dpfButtons, "dpf")
    processButtons(ipfButtons, "ipf")
  })

  video.addEventListener("timeupdate", function () {
    let currentFrame = Math.round(video.currentTime * fps)
    currentFrameElement.textContent = "Current Frame: " + currentFrame
    currentTimecodeElement.textContent =
      "Current Timecode: " + frameToTimecode(currentFrame, fps)
  })

  let frameTime = 1 / fps

  function seek(event, frame) {
    event.preventDefault()
    video.currentTime = (frame + 1) / fps
    video.focus()
    if (!event.target.dataset.colorChanged) {
      event.target.style.backgroundColor = "rgb(" + CONSTANTS.CYAN.join() + ")"
      event.target.dataset.colorChanged = true
    }
    let buttonText = event.target.textContent
    let match = buttonText.match(/From\s+(.*?)\s+frame/)
    if (match) {
      navigator.clipboard.writeText(match[1])
    }
  }

  function togglePlay() {
    if (video.paused) {
      video.play()
    } else {
      video.pause()
    }
  }

  function updatePlayBtn() {
    playBtn.innerHTML = video.paused ? "Play" : "Pause"
  }

  playBtn.addEventListener("click", togglePlay)
  video.addEventListener("click", togglePlay)
  video.addEventListener("play", updatePlayBtn)
  video.addEventListener("pause", updatePlayBtn)

  lastBtn.addEventListener("click", function () {
    video.currentTime = Math.floor((totalFrames * 0.99) / fps)
  })

  function handleProgress() {
    const progressPercent = (video.currentTime / video.duration) * 100
    progressOverlay.style.width = `${progressPercent}%`
  }

  function jump(e) {
    const rect = seekOverlay.getBoundingClientRect()
    const clickX = e.clientX - rect.left
    const clickPercent = clickX / rect.width
    const jumpTime = clickPercent * video.duration
    video.currentTime = jumpTime
  }

  video.addEventListener("timeupdate", handleProgress)
  seekOverlay.addEventListener("click", jump)

  let mousedown = false
  seekOverlay.addEventListener("mousedown", () => (mousedown = true))
  seekOverlay.addEventListener("mousemove", (e) => mousedown && jump(e))
  seekOverlay.addEventListener("mouseup", () => (mousedown = false))

  document.addEventListener("keydown", function (event) {
    switch (event.key) {
      case ",":
        video.currentTime = Math.max(video.currentTime - frameTime, 0)
        break
      case ".":
        video.currentTime = Math.min(
          video.currentTime + frameTime,
          video.duration
        )
        break
      case "k":
      case " ":
        togglePlay()
        video.playbackRate = 1
        break
      case "l":
        if (video.paused) togglePlay()
        else video.playbackRate += 0.5
        break
      case "j":
        if (!video.paused) video.playbackRate -= 0.25
        break
      case "h":
        video.currentTime -= 3
        break
      case "ArrowLeft":
        video.currentTime -= 1
        break
      case "ArrowRight":
        video.currentTime += 1
        break
    }
    playSpeed.innerHTML = "Speed: " + video.playbackRate + "x"
  })

  getTimecode.addEventListener("click", () => {
    navigator.clipboard.writeText(
      frameToTimecode(Math.round(video.currentTime * fps), fps)
    )
  })

  getFCPXTimecode.addEventListener("click", () => {
    navigator.clipboard.writeText(
      frameToFCPXTimecode(Math.round(video.currentTime * fps), fps)
    )
  })
})
