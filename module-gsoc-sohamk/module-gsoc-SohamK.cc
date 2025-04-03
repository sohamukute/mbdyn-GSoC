/*
 * MBDyn (C) is a multibody analysis code.
 * http://www.mbdyn.org
 *
 * Copyright (C) 1996-2022
 *
 * Pierangelo Masarati	<masarati@aero.polimi.it>
 * Paolo Mantegazza	<mantegazza@aero.polimi.it>
 *
 * Dipartimento di Scienze e Tecnologie Aerospaziali - Politecnico di Milano
 * via La Masa, 34 - 20156 Milano, Italy
 * http://www.aero.polimi.it
 *
 * Changing this copyright notice is forbidden.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation (version 2 of the License).
 *
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * Custom constitutive law module for MBDyn - Nonlinear Spring with Saturation
 * This module implements a spring with saturation limits on the force
 */

 #include "mbconfig.h"    

 #include <iostream>
 #include <cfloat>
 
 #include "dataman.h"
 #include "constltp.h"
 #include "constlaw.h"
 
 // Class for the nonlinear spring with saturation
 class NonlinearSatSpring
 : public ConstitutiveLaw1D {
 private:
     doublereal m_dK;            // Spring stiffness
     doublereal m_dMaxForce;     // Maximum (absolute) force
     doublereal m_dPreload;      // Initial preload
     
 public:
     // Constructor
     NonlinearSatSpring(doublereal dK,
                        doublereal dMaxForce,
                        doublereal dPreload = 0.0)
     : m_dK(dK), m_dMaxForce(dMaxForce), m_dPreload(dPreload)
     {
         // Ensure positive values for parameters
         if (m_dK <= 0.0) {
             silent_cerr("NonlinearSatSpring: invalid stiffness " << m_dK << std::endl);
             throw ErrGeneric(MBDYN_EXCEPT_ARGS);
         }
         
         if (m_dMaxForce <= 0.0) {
             silent_cerr("NonlinearSatSpring: invalid maximum force " << m_dMaxForce << std::endl);
             throw ErrGeneric(MBDYN_EXCEPT_ARGS);
         }
     }
     
     // Destructor
     virtual ~NonlinearSatSpring(void) {
         // Nothing to do
     }
     
     // Clone method (virtual constructor)
     virtual ConstitutiveLaw1D* pCopy(void) const {
         ConstitutiveLaw1D* pCL = NULL;
         
         SAFENEWWITHCONSTRUCTOR(pCL,
                                NonlinearSatSpring,
                                NonlinearSatSpring(m_dK, m_dMaxForce, m_dPreload));
         return pCL;
     }
     
     // Get the force given strain & strain rate
     virtual doublereal GetF(const doublereal& Eps) const {
         // Calculate linear spring force
         doublereal F = m_dK * Eps + m_dPreload;
         
         // Apply saturation
         if (F > m_dMaxForce) {
             F = m_dMaxForce;
         } else if (F < -m_dMaxForce) {
             F = -m_dMaxForce;
         }
         
         return F;
     }
     
     // Get the force derivative given strain & strain rate
     virtual doublereal GetFDE(const doublereal& Eps) const {
         // Calculate linear spring force
         doublereal F = m_dK * Eps + m_dPreload;
         
         // If we're in the saturation region, the derivative is zero
         if (F > m_dMaxForce || F < -m_dMaxForce) {
             return 0.0;
         }
         
         // Otherwise, return the stiffness
         return m_dK;
     }
     
     // Get the force derivative wrt strain rate (zero for a spring)
     virtual doublereal GetFDEP(const doublereal& /* Eps */ ) const {
         return 0.0;
     }
     
     // Initialize internal state
     virtual void Update(const doublereal& /* Eps */ , const doublereal& /* EpsPrime */ ) {
         // Nothing to do for this constitutive law
     }

     // Get the energy given strain & strain rate
     virtual doublereal GetE(const doublereal& Eps) const {
         // Calculate linear spring energy
         doublereal F = m_dK * Eps + m_dPreload;
         doublereal energy = 0.5 * m_dK * Eps * Eps + m_dPreload * Eps;

         // Apply saturation
         if (F > m_dMaxForce) {
             energy = m_dMaxForce * (Eps - m_dMaxForce / m_dK) + 0.5 * m_dMaxForce * m_dMaxForce / m_dK;
         } else if (F < -m_dMaxForce) {
             energy = -m_dMaxForce * (Eps + m_dMaxForce / m_dK) + 0.5 * m_dMaxForce * m_dMaxForce / m_dK;
         }

         return energy;
     }
 };
 
 // Parse function to read parameters from the input file
 static ConstitutiveLaw1D*
 ReadNonlinearSatSpring(DataManager* pDM,
                       MBDynParser& HP,
                       ConstLaw1DOwner* pCL)
 {
     ConstitutiveLaw1D* pCL1D = 0;
     
     // Read stiffness parameter
     doublereal dK = HP.GetReal();
     if (dK <= 0.0) {
         silent_cerr("NonlinearSatSpring: invalid stiffness " 
                    << dK << " at line " << HP.GetLineData() << std::endl);
         throw ErrGeneric(MBDYN_EXCEPT_ARGS);
     }
     
     // Read maximum force parameter
     doublereal dMaxForce = HP.GetReal();
     if (dMaxForce <= 0.0) {
         silent_cerr("NonlinearSatSpring: invalid maximum force " 
                    << dMaxForce << " at line " << HP.GetLineData() << std::endl);
         throw ErrGeneric(MBDYN_EXCEPT_ARGS);
     }
     
     // Read preload if provided (optional)
     doublereal dPreload = 0.0;
     if (HP.IsKeyWord("preload")) {
         dPreload = HP.GetReal();
     }
     
     // Create the constitutive law
     SAFENEWWITHCONSTRUCTOR(pCL1D,
                           NonlinearSatSpring,
                           NonlinearSatSpring(dK, dMaxForce, dPreload));
     
     // Set the owner, if provided
     if (pCL) {
         pCL->Set(pCL1D);
     }
     
     return pCL1D;
 }
 
 // Module initialization function
 extern "C" int
 module_init(const char *module_name, void *pdm, void *php)
 {
     DataManager* pDM = (DataManager*)pdm;
     MBDynParser& HP = *((MBDynParser*)php);
     
     // Add the constitutive law to the set of available laws
     if (!SetCL1D("nonlinear" "saturated" "spring", ReadNonlinearSatSpring)) {
         silent_cerr("NonlinearSatSpring: "
                    "module_init(" << module_name << ") failed" << std::endl);
         return -1;
     }
     
     return 0;
 }