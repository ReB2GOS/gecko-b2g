// <gaia-statusbar> custom element.

class StatusBar extends HTMLElement {
  constructor() {
    super();

    let shadow = this.attachShadow({ mode: "open" });

    let container = document.createElement("div");
    container.innerHTML = `
           <link rel="stylesheet" href="style/statusbar.css">
           <div class="left">
             Current page title that could be way too long to fit so we need to clip it some way.
           </div>
           <div class="right">
             <span class="battery-percent"></span>
             <span class="clock">${this.local_time()}</span>
           </div>
          `;

    shadow.appendChild(container);

    this.left = shadow.querySelector(".left");
  
    this.clock = shadow.querySelector(".clock");
    window.setInterval(() => {
      this.clock.textContent = this.local_time();
    }, 10000);

    if (!navigator.getBattery) {
      console.error("navigator.getBattery is not implemented!");
      return;
    }

    // Setup the battery listeners.
    this.battery_percent = shadow.querySelector(".battery-percent");
    navigator.getBattery().then(
      battery => {
        // Displauy the initial state.
        this.update_battery_info(battery);

        // Listen for charging and level changes.
        battery.addEventListener("levelchange", () => {
          this.update_battery_info(battery);
        });
        battery.addEventListener("chargingchange", () => {
          this.update_battery_info(battery);
        });
      },
      () => {
        // No battery information available, keep the icon hidden.
        console.error("No battery information available.");
      }
    );
  }

  update_battery_info(battery) {
    this.battery_percent.textContent = `[${Math.round(battery.level * 100)}%]`;
  }

  // Returns the local time properly formatted.
  local_time() {
    let now = new Date();
    let minutes = now.getMinutes();
    return `${now.getHours()}:${minutes < 10 ? "0" : ""}${minutes}`;
  }

  update_state(state) {
      this.left.textContent = state.title;
      if (state.secure == "secure" || state.url.startsWith("chrome://")) {
          this.left.classList.remove("insecure");
      } else {
          this.left.classList.add("insecure");
      }
  }
}

customElements.define("gaia-statusbar", StatusBar);
