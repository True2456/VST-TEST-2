# EwolFX Framework Data Flow

The following Mermaid diagram outlines the data flow and class architecture of an EwolFX plugin. Small language models can use this diagram to understand the boundary between the Audio thread (DSP) and the UI thread (Graphics), and how the framework wraps IPlug2.

```mermaid
flowchart TD
    %% Define Styles
    classDef iplug fill:#1a1a1a,stroke:#333,stroke-width:2px,color:#aaa
    classDef ewol fill:#161616,stroke:#2a2a2a,stroke-width:2px,color:#e0e0e0
    classDef plugin fill:#222,stroke:#4a90e2,stroke-width:2px,color:#fff
    classDef thread_audio fill:#2d1b1b,stroke:#e74c3c,stroke-width:2px,color:#fff
    classDef thread_ui fill:#1b2a2d,stroke:#3498db,stroke-width:2px,color:#fff

    %% Components
    subgraph IPlug2 ["IPlug2 Engine (Host Wrapper)"]
        IP["iplug::Plugin"]:::iplug
        IG["iplug::igraphics::IGraphics"]:::iplug
    end

    subgraph EWOL ["EWOL Template Framework"]
        EWP["EWOL::EWPlugin"]:::ewol
        EWC["EWControls (EWKnob, EWPanel)"]:::ewol
        EWCol["EWColors (Palette)"]:::ewol
    end

    subgraph CustomPlugin ["Your Plugin (e.g., EwolFX_Gain)"]
        PluginClass["EwolFX_Gain Class"]:::plugin
        
        subgraph DSP ["Audio Thread (Real-time)"]
            PB["ProcessBlock()"]:::thread_audio
        end
        
        subgraph GUI ["UI Thread (Async)"]
            SL["SetupLayout()"]:::thread_ui
        end
    end

    %% Relationships
    IP <|-- EWP : Inherits
    EWP <|-- PluginClass : Inherits

    IG <-- EWP : mMakeGraphicsFunc / mLayoutFunc setup
    EWC --> EWCol : Styles via GetEwolStyle()
    
    PluginClass --> EWC : Instantiates controls in SetupLayout()
    PluginClass --> EWCol : Provides GetAccent()
    
    %% Data Flow
    DAW["Host DAW (Ableton, Logic)"] --> |Audio Buffers| PB
    DAW --> |Parameter Automation| IP
    
    IP --> |Normalized Values (0.0 to 1.0)| EWC
    EWC --> |User Interactions| IP
    
    IP -.-> |mParam->Value()| PB
```

### Component Details

1. **`EWOL::EWPlugin`**: The base class. It inherits from `iplug::Plugin` and abstracts away the tedious setup of graphics contexts and UI window boilerplate.
2. **`EWColors` & `EWControls`**: These static configurations intercept IPlug2's standard components (like `IVKnobControl`) and inject a beautiful, uniform Dark Mode aesthetic. This prevents local models from trying to generate fragile CSS or raw RGB arrays.
3. **`SetupLayout()`**: The single entry point for UI generation in your custom plugin. It is called exactly once when the plugin window opens.
4. **`ProcessBlock()`**: The real-time audio thread. It retrieves sanitized, thread-safe parameters directly from IPlug2 to process audio buffers. Never call UI or memory-allocation functions here.
