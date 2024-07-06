var video = document.getElementById('video');
var currentFrameElement = document.getElementById('currentFrame');
var fps = {{fps}}

video.addEventListener('timeupdate', function() {
    var currentFrame = Math.round(video.currentTime * fps);
    currentFrameElement.textContent = 'Current Frame: ' + currentFrame;
});

var frameTime = 1 / fps;
document.addEventListener('keydown', function(event) {
    switch (event.key) {
        case ',':
            video.currentTime = Math.max(video.currentTime - frameTime, 0);
            break;
        case '.':
            video.currentTime = Math.min(video.currentTime + frameTime, video.duration);
            break;
    }
});

function seek(frame) {
    event.preventDefault();
    video.currentTime = (frame + 1) / fps;
    video.focus();

    var buttonText = event.target.textContent;
    var match = buttonText.match(/From\s+(.*?)\s+to/);
    if (match) {
        navigator.clipboard.writeText(match[1]);
    }
}
