window.addEventListener("keydown", (e) => {
  if (e.keyCode === 32 && e.target === document.body) {
    e.preventDefault();
  }
});

function SaveHighScore(newHighScore) {
  console.log(`Saving: ${newHighScore} from cpp!`);
  localStorage.setItem("HighScore", newHighScore);
}

function GetCurrentHighScore() {
  return parseInt(localStorage.getItem("HighScore"));
}
