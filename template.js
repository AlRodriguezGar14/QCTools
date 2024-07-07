var video = document.getElementById('video');
var playBtn = document.getElementById('play');
var lastBtn = document.getElementById('last90');
var progress = document.querySelector('.progress');
var progressBar = document.getElementById('progress-bar');
var errorsBar = document.getElementById('errors-bar');
var dpfButtons = document.getElementsByClassName('dpf');
var ipfButtons = document.getElementsByClassName('ipf');
var playSpeed = document.getElementById('play-speed');
var currentFrameElement = document.getElementById('currentFrame');
var fps = {{m_fps}};
var totalFrames;


function paintError(frame, error, color) {
    if (!totalFrames) return;

    const positionPercent = (frame / totalFrames) * 100;

    const errorElement = document.createElement('div');
    errorElement.style.position = 'absolute';
    errorElement.style.left = `${positionPercent}%`;
    errorElement.style.top = '0';
    errorElement.style.width = '2px';
    errorElement.style.height = '100%';

    errorElement.style.backgroundColor = color;

    errorsBar.appendChild(errorElement);
}

function processButtons(buttons, errorType) {
    Array.from(buttons).forEach(button => {
        const frameMatch = button.textContent.match(/frame: (\d+)/);
        if (frameMatch) {
            const frame = parseInt(frameMatch[1], 10);
            button.onclick = (event) => seek(event, frame);
            switch (errorType) {
                case 'dpf':
                    button.style.backgroundColor = '#973131';
                    break;
                case 'ipf':
                    button.style.backgroundColor = '#597445';
                    break;
            }
            paintError(frame, errorType, button.style.backgroundColor);
        }
    });
}
video.addEventListener('loadedmetadata', function() {
    totalFrames = Math.round(video.duration * fps);
    let videoWidth = video.offsetWidth;
    progress.style.width = videoWidth + 'px';

    processButtons(dpfButtons, 'dpf');
    processButtons(ipfButtons, 'ipf');
});

video.addEventListener('timeupdate', function() {
    let currentFrame = Math.round(video.currentTime * fps);
    currentFrameElement.textContent = 'Current Frame: ' + currentFrame;
});

var frameTime = 1 / fps;

function seek(event, frame) {
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

lastBtn.addEventListener('click', function() {
    video.currentTime = Math.floor((totalFrames * 0.99) / fps);
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
        case ' ':
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
        case "ArrowLeft":
            video.currentTime -= 1;
            break;
        case "ArrowRight":
            video.currentTime += 1;
            break;
    }
    playSpeed.innerHTML = 'Speed: ' + video.playbackRate + 'x';
});

