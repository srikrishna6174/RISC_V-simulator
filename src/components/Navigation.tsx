
import React from "react";
import { Link, useLocation } from "react-router-dom";
import ThemeToggle from "./ThemeToggle";

const Navigation = () => {
  const location = useLocation();

  const isActive = (path: string) => location.pathname === path;

  return (
    <header className="border-b border-border/30 backdrop-blur-sm sticky top-0 z-10 bg-background/80">
      <div className="max-w-7xl mx-auto px-4 sm:px-6 py-6 flex items-center justify-between">
        <div className="animate-fade-in">
          <h1 className="text-3xl sm:text-4xl font-bold tracking-tight bg-gradient-to-r from-foreground to-foreground/70 bg-clip-text text-transparent">
            RISC-V Cycle Simulator
          </h1>
          <p className="text-muted-foreground mt-2 max-w-2xl">
            {location.pathname === "/" && "Cycles Overview"}
            {location.pathname === "/traverse" && "Traverse execution cycles"}
            {location.pathname === "/pipeline" && "Pipeline Simulation"}
            {location.pathname === "/pipeline-overview" && "Pipeline Overview"}
            {location.pathname === "/memory" && "Memory View"}
            {location.pathname === "/statistics" && "Performance Analysis"}
          </p>
        </div>
        <nav className="flex items-center gap-4">
          <Link 
            to="/" 
            className={`text-sm transition-colors ${
              isActive("/") ? "text-foreground font-medium" : "text-muted-foreground hover:text-foreground"
            }`}
          >
            Overview
          </Link>
          <Link 
            to="/traverse" 
            className={`text-sm transition-colors ${
              isActive("/traverse") ? "text-foreground font-medium" : "text-muted-foreground hover:text-foreground"
            }`}
          >
            Traverse
          </Link>
          <Link 
            to="/pipeline" 
            className={`text-sm transition-colors ${
              isActive("/pipeline") ? "text-foreground font-medium" : "text-muted-foreground hover:text-foreground"
            }`}
          >
            Pipeline Simulation
          </Link>
          <Link 
            to="/pipeline-overview" 
            className={`text-sm transition-colors ${
              isActive("/pipeline-overview") ? "text-foreground font-medium" : "text-muted-foreground hover:text-foreground"
            }`}
          >
            Pipeline Overview
          </Link>
          <Link 
            to="/memory" 
            className={`text-sm transition-colors ${
              isActive("/memory") ? "text-foreground font-medium" : "text-muted-foreground hover:text-foreground"
            }`}
          >
            Memory
          </Link>
          <Link 
            to="/statistics" 
            className={`text-sm transition-colors ${
              isActive("/statistics") ? "text-foreground font-medium" : "text-muted-foreground hover:text-foreground"
            }`}
          >
            Statistics
          </Link>
          <ThemeToggle />
        </nav>
      </div>
    </header>
  );
};

export default Navigation;

