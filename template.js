var video = document.getElementById('video');
var playBtn = document.getElementById('play');
var progress = document.querySelector('.progress');
var progressBar = document.querySelector('.progress-bar');
var playSpeed = document.getElementById('play-speed');
var currentFrameElement = document.getElementById('currentFrame');
var fps = {{m_fps}}

video.addEventListener('timeupdate', function() {
    let currentFrame = Math.round(video.currentTime * fps);
    currentFrameElement.textContent = 'Current Frame: ' + currentFrame;
});

var frameTime = 1 / fps;

function seek(frame) {
    event.preventDefault();
    video.currentTime = (frame + 1) / fps;
    video.focus();

    let buttonText = event.target.textContent;
    let match = buttonText.match(/From\s+(.*?)\s+frame/);
    if (match) {
        navigator.clipboard.writeText(match[1]);
    }
}


function togglePlay() {
    if (video.paused) {
        video.play();
    } else {
        video.pause();
    }
}
function updatePlayBtn() {
    playBtn.innerHTML = video.paused ? "Play" : "Pause";
}

playBtn.addEventListener('click', togglePlay);
video.addEventListener('click', togglePlay);
video.addEventListener('play', updatePlayBtn);
video.addEventListener('pause', updatePlayBtn);


video.addEventListener("loadedmetadata", function() {
  let videoWidth = video.offsetWidth;
  progress.style.width = videoWidth + 'px';
});
function handleProgress() {
    const progressPercent = (video.currentTime / video.duration) * 100;
    progressBar.style.width = `${progressPercent}%`;
}
function jump(e) {
    const jumpTime = (e.offsetX / progress.offsetWidth) * video.duration;
    video.currentTime = jumpTime;
}
video.addEventListener('timeupdate', handleProgress);
progress.addEventListener('click', jump);
let mousedown = false;
progress.addEventListener('mousedown', () => mousedown = true);
progress.addEventListener('mousemove', (e) => mousedown && jump(e));
progress.addEventListener('mouseup', () => mousedown = false);
document.addEventListener('keydown', function(event) {
    switch (event.key) {
        case ',':
            video.currentTime = Math.max(video.currentTime - frameTime, 0);
            break;
        case '.':
            video.currentTime = Math.min(video.currentTime + frameTime, video.duration);
            break;
        case 'k':
        case ' ': // event.preventDefault() if it doesn't feel comfortable as it is now
            togglePlay();
            video.playbackRate = 1;
            break;
        case 'l':
            if (video.paused)
                togglePlay();
            else
                video.playbackRate += 0.5;
            break;
        case 'j':
            if (!video.paused)
                video.playbackRate -= 0.25;
            break;
        case 'h':
            video.currentTime -= 3;
            break;
    }
    playSpeed.innerHTML = 'Speed: ' + video.playbackRate + 'x';
});
