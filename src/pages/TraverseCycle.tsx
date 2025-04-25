import React, { useEffect, useState } from "react";
import Navigation from "@/components/Navigation";
import { Button } from "@/components/ui/button";
import CycleCard from "@/components/CycleCard";
import { ArrowLeft, ArrowRight, SkipForward } from "lucide-react";

interface Log {
  stage: string;
  message: string;
}

interface Register {
  reg: string;
  value: string;
}

interface Memory {
  address: string;
  value: string;
}

interface CycleData {
  cycle: number;
  logs: Log[];
  registers?: Register[];
  memory?: Memory[];
}

const TraverseCycle: React.FC = () => {
  const [cycleData, setCycleData] = useState<CycleData[]>([]);
  const [currentCycleIndex, setCurrentCycleIndex] = useState(0);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await fetch("../web.txt");
        if (!response.ok) {
          throw new Error("Failed to fetch data");
        }
        
        const text = await response.text();
        const data = JSON.parse(text);
        
        setCycleData(data);
        setLoading(false);
      } catch (err) {
        console.error("Error fetching data:", err);
        setError("Failed to load simulation data. Please check if web.txt is properly formatted.");
        setLoading(false);
      }
    };

    fetchData();
  }, []);

  const goToPreviousCycle = () => {
    if (currentCycleIndex > 0) {
      setCurrentCycleIndex(prevIndex => prevIndex - 1);
    }
  };

  const goToNextCycle = () => {
    if (currentCycleIndex < cycleData.length - 1) {
      setCurrentCycleIndex(prevIndex => prevIndex + 1);
    }
  };

  const goToLastCycle = () => {
    if (cycleData.length > 0) {
      setCurrentCycleIndex(cycleData.length - 1);
    }
  };

  return (
    <div className="min-h-screen w-full bg-background text-foreground transition-colors duration-300">
      <Navigation />
      <main className="max-w-7xl mx-auto px-4 sm:px-6 py-8 pb-20">
        {loading ? (
          <div className="flex items-center justify-center min-h-[50vh]">
            <div className="text-center animate-pulse">
              <p className="text-lg">Loading simulation data...</p>
            </div>
          </div>
        ) : error ? (
          <div className="bg-destructive/10 text-destructive p-4 rounded-lg">
            <p>{error}</p>
          </div>
        ) : cycleData.length > 0 ? (
          <>
            <div className="flex items-center justify-between mb-6">
              <Button 
                onClick={goToPreviousCycle} 
                disabled={currentCycleIndex === 0}
                variant="outline"
              >
                <ArrowLeft className="mr-2 h-4 w-4" /> Previous
              </Button>
              <span className="text-lg font-medium">
                Cycle {cycleData[currentCycleIndex]?.cycle} of {cycleData[cycleData.length - 1]?.cycle}
              </span>
              <div className="flex gap-2">
                <Button 
                  onClick={goToNextCycle} 
                  disabled={currentCycleIndex === cycleData.length - 1}
                  variant="outline"
                >
                  Next <ArrowRight className="ml-2 h-4 w-4" />
                </Button>
                <Button 
                  onClick={goToLastCycle} 
                  disabled={currentCycleIndex === cycleData.length - 1}
                >
                  End <SkipForward className="ml-2 h-4 w-4" />
                </Button>
              </div>
            </div>
            <CycleCard 
              key={cycleData[currentCycleIndex]?.cycle} 
              cycle={cycleData[currentCycleIndex]?.cycle} 
              logs={cycleData[currentCycleIndex]?.logs} 
              registers={cycleData[currentCycleIndex]?.registers}
              memory={cycleData[currentCycleIndex]?.memory}
            />
          </>
        ) : (
          <div className="bg-secondary/30 p-6 rounded-lg text-center">
            <p className="text-lg">No simulation data available.</p>
          </div>
        )}
      </main>
    </div>
  );
};

export default TraverseCycle;
