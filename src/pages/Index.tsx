
import React, { useEffect, useState } from "react";
import Navigation from "@/components/Navigation";
import CycleCard from "@/components/CycleCard";

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

const Index: React.FC = () => {
  const [cycleData, setCycleData] = useState<CycleData[]>([]);
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
        ) : (
          <div className="grid grid-cols-1 gap-8">
            {cycleData.map((data) => (
              <CycleCard 
                key={data.cycle} 
                cycle={data.cycle} 
                logs={data.logs} 
                registers={data.registers}
                memory={data.memory}
              />
            ))}
          </div>
        )}
      </main>
    </div>
  );
};

export default Index;
